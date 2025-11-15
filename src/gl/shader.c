#include "shader.h"

#include "../glx/hardext.h"
#include "debug.h"
#include "fpe_shader.h"
#include "init.h"
#include "gl4es.h"
#include "glstate.h"
#include "loader.h"
#include "shaderconv.h"
#include "vgpu/shaderconv.h"
#include "glsl/glsl_for_es.h"
#include <ctype.h>
#include <string.h>

// #define DEBUG
#ifdef DEBUG
#define DBG(a) a
#else
#define DBG(a)
#endif

KHASH_MAP_IMPL_INT(shaderlist, shader_t*);

GLuint APIENTRY_GL4ES gl4es_glCreateShader(GLenum shaderType) {
    DBG(SHUT_LOGD("glCreateShader(%s)\n", PrintEnum(shaderType)))
    // sanity check
    if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER  && shaderType != GL_GEOMETRY_SHADER) {
        DBG(SHUT_LOGD("Invalid shader type\n"))
        errorShim(GL_INVALID_ENUM);
        return 0;
    }
    static GLuint lastshader = 0;
    GLuint shader;
    // create the shader
    LOAD_GLES2(glCreateShader);
    if (gles_glCreateShader) {
        shader = gles_glCreateShader(shaderType);
        if (!shader) {
            DBG(SHUT_LOGD("Failed to create shader\n"))
            errorGL();
            return 0;
        }
    } else {
        shader = ++lastshader;
        noerrorShim();
    }
    // store the new empty shader in the list for later use
    khint_t k;
    int ret;
    khash_t(shaderlist)* shaders = glstate->glsl->shaders;
    k = kh_get(shaderlist, shaders, shader);
    shader_t* glshader = NULL;
    if (k == kh_end(shaders)) {
        k = kh_put(shaderlist, shaders, shader, &ret);
        glshader = kh_value(shaders, k) = (shader_t*)calloc(1, sizeof(shader_t));
    } else {
        glshader = kh_value(shaders, k);
    }
    glshader->id = shader;
    glshader->type = shaderType;
    if (glshader->source) {
        free(glshader->source);
        glshader->source = NULL;
    }
    glshader->need.need_texcoord = -1;

    // all done
    return shader;
}

void actually_deleteshader(GLuint shader) {
    khint_t k;
    khash_t(shaderlist)* shaders = glstate->glsl->shaders;
    k = kh_get(shaderlist, shaders, shader);
    if (k != kh_end(shaders)) {
        shader_t* glshader = kh_value(shaders, k);
        if (glshader->deleted && !glshader->attached) {
            kh_del(shaderlist, shaders, k);
            if (glshader->source) free(glshader->source);
            if (glshader->converted) free(glshader->converted);
            free(glshader);
        }
    }
}

void actually_detachshader(GLuint shader) {
    khint_t k;
    khash_t(shaderlist)* shaders = glstate->glsl->shaders;
    k = kh_get(shaderlist, shaders, shader);
    if (k != kh_end(shaders)) {
        shader_t* glshader = kh_value(shaders, k);
        if ((--glshader->attached) < 1 && glshader->deleted) actually_deleteshader(shader);
    }
}

void APIENTRY_GL4ES gl4es_glDeleteShader(GLuint shader) {
    DBG(SHUT_LOGD("glDeleteShader(%d)\n", shader))
    // sanity check...
    CHECK_SHADER(void, shader)
    // delete the shader from the list
    if (!glshader) {
        noerrorShim();
        return;
    }
    glshader->deleted = 1;
    noerrorShim();
    if (!glshader->attached) {
        actually_deleteshader(shader);

        // delete the shader in GLES2 hardware (if any)
        LOAD_GLES2(glDeleteShader);
        if (gles_glDeleteShader) {
            errorGL();
            gles_glDeleteShader(shader);
        }
    }
}

void APIENTRY_GL4ES gl4es_glCompileShader(GLuint shader) {
    DBG(SHUT_LOGD("glCompileShader(%d)\n", shader))
    // look for the shader
    CHECK_SHADER(void, shader)

    glshader->compiled = 1;
    LOAD_GLES2(glCompileShader);
    if (gles_glCompileShader) {
        gles_glCompileShader(glshader->id);
        errorGL();
        // if(globals4es.logshader) {
        { // always log the error of shader
            // get compile status and print shaders sources if compile fail...
            LOAD_GLES2(glGetShaderiv);
            LOAD_GLES2(glGetShaderInfoLog);
            GLint status = 0;
            gles_glGetShaderiv(glshader->id, GL_COMPILE_STATUS, &status);
            if (status != GL_TRUE) {
                DBG(SHUT_LOGD("LIBGL: Error while compiling shader %d. Original source is:\n%s\n=======\n", glshader->id,
                          glshader->source);)
                DBG(SHUT_LOGD("ShaderConv Source is:\n%s\n=======\n", glshader->converted);)
                char tmp[500];
                GLint length;
                gles_glGetShaderInfoLog(glshader->id, 500, &length, tmp);
                DBG(SHUT_LOGD("Compiler message is\n%s\nLIBGL: End of Error log\n", tmp);)
            }
        }
    } else
        noerrorShim();
}

bool can_run_essl3(int esversion, const char* glsl) {
    int glsl_version = 0;
    if (strncmp(glsl, "#version 100", 12) == 0) {
        return true;
    } else if (strncmp(glsl, "#version 300 es", 15) == 0) {
        return true;
    } else if (strncmp(glsl, "#version 310 es", 15) == 0) {
        glsl_version = 310;
    } else if (strncmp(glsl, "#version 320 es", 15) == 0) {
        glsl_version = 320;
    } else {
        return false;
    }
    if (esversion >= glsl_version) {
        return true;
    } else {
        return false;
    }
}

bool is_direct_shader(char* glsl) {
    bool es2_ability = glstate->glsl->es2 && !strncmp(glsl, "#version 100", 12);
    bool es3_ability = globals4es.es >= 3 &&
                       can_run_essl3(globals4es.esversion != -1 ? globals4es.esversion : globals4es.es * 100, glsl);
    return es2_ability || es3_ability;
}

char* replace_version_line(const char* text) {
    if (!text) return NULL;

    const char* line_start = text;
    const char* p = text;
    const char* replace_str = "#version 150 compatibility\n";
    size_t replace_len = strlen(replace_str);

    while (*p) {
        if (*p == '\n' || *(p + 1) == '\0') {
            size_t len = p - line_start + (*(p + 1) == '\0' ? 1 : 0);

            size_t i = 0;
            while (i < len && isspace((unsigned char)line_start[i]))
                i++;

            if (strncmp(line_start + i, "#version", 8) == 0) {
                size_t new_len = strlen(text) - len + replace_len;
                char* new_text = malloc(new_len + 1);
                if (!new_text) return NULL;

                memcpy(new_text, text, line_start - text);
                memcpy(new_text + (line_start - text), replace_str, replace_len);
                strcpy(new_text + (line_start - text) + replace_len, p + (*(p + 1) == '\0' ? 0 : 1));

                return new_text;
            }

            line_start = p + 1;
        }
        p++;
    }

    return strdup(text);
}

char* replace_version_line_460(const char* text) {
    const char* new_version = "#version 460\n";
    if (!text || !new_version) return NULL;

    const char* p = text;
    const char* line_start = text;
    size_t text_len = strlen(text);
    size_t new_version_len = strlen(new_version);

    while (*p) {
        const char* line_end = strchr(p, '\n');
        const char* next_line = NULL;
        if (line_end) {
            next_line = line_end + 1;
        } else {
            line_end = text + text_len;
            next_line = line_end;
        }

        const char* cur = p;
        while (cur < line_end && isspace((unsigned char)*cur) && *cur != '\n' && *cur != '\r')
            cur++;

        const char* tok = "#version";
        size_t toklen = strlen(tok);
        if ((size_t)(line_end - cur) >= toklen && strncmp(cur, tok, toklen) == 0) {
            const char next_char = (cur + toklen < line_end) ? *(cur + toklen) : '\0';
            if (next_char == '\0' || isspace((unsigned char)next_char)) {
                size_t prefix_len = cur - text;
                prefix_len = p - text;

                const char* suffix_start = next_line;

                size_t suffix_len = strlen(suffix_start);
                size_t new_total = prefix_len + new_version_len + suffix_len;
                char* out = (char*)malloc(new_total + 1);
                if (!out) return NULL;

                memcpy(out, text, prefix_len);
                memcpy(out + prefix_len, new_version, new_version_len);
                memcpy(out + prefix_len + new_version_len, suffix_start, suffix_len);
                out[new_total] = '\0';
                return out;
            }
        }
        p = next_line;
    }

    return strdup(text);
}

static char* force_replace_version(const char* text, const char* new_version_line) {
    if (!text) return NULL;

    const char* version_tag = "#version";
    const char* p = text;
    // Skip leading whitespace
    while (*p && isspace((unsigned char)*p)) p++;

    // Check if the source starts with #version
    if (strncmp(p, version_tag, strlen(version_tag)) == 0) {
        const char* first_line_end = strchr(p, '\n');
        if (!first_line_end) {
            first_line_end = p + strlen(p); // Handle single-line shaders
        }

        // Calculate the start of the rest of the code
        const char* rest_of_code = first_line_end;
        if (*rest_of_code == '\n') {
            rest_of_code++; // Move past the newline character
        }

        // Allocate memory and build the new shader string
        size_t new_version_len = strlen(new_version_line);
        size_t rest_len = strlen(rest_of_code);
        char* new_text = (char*)malloc(new_version_len + 1 + rest_len + 1); // +1 for newline, +1 for null terminator
        if (!new_text) return NULL;

        strcpy(new_text, new_version_line);
        strcat(new_text, "\n");
        strcat(new_text, rest_of_code);

        return new_text;
    }

    // If #version is not found at the beginning, insert the new version line at the top
    size_t text_len = strlen(text);
    size_t new_version_len = strlen(new_version_line);
    char* new_text = (char*)malloc(new_version_len + 1 + text_len + 1);
    if (!new_text) return NULL;

    strcpy(new_text, new_version_line);
    strcat(new_text, "\n");
    strcat(new_text, text);

    return new_text;
}

static char* insert_after_preamble(const char* text, const char* insert_str) {
    if (!text || !insert_str) return NULL;

    const char* p = text;
    const char* last_preamble_pos = text;

    // Find the end of the preamble (all lines starting with #, comments, or are empty)
    while (*p) {
        const char* line_start = p;

        // Skip leading whitespace
        while (*p && (*p == ' ' || *p == '\t')) p++;

        if (*p == '#' || (*p == '/' && *(p+1) == '/') || *p == '\n' || *p == '\r' || *p == '\0') {
            // This line is a preprocessor, comment, or empty. Continue.
            const char* line_end = strchr(line_start, '\n');
            if (line_end) {
                p = line_end + 1;
                last_preamble_pos = p;
            } else {
                // End of string
                last_preamble_pos = p + strlen(p);
                break;
            }
        } else {
            // First line of actual code found. We should insert before it.
            break;
        }
    }

    // Now, build the new string
    size_t head_len = last_preamble_pos - text;
    size_t insert_len = strlen(insert_str);
    size_t tail_len = strlen(last_preamble_pos);

    char* new_text = (char*)malloc(head_len + insert_len + tail_len + 1);
    if (!new_text) return NULL;

    memcpy(new_text, text, head_len);
    memcpy(new_text + head_len, insert_str, insert_len);
    memcpy(new_text + head_len + insert_len, last_preamble_pos, tail_len);
    new_text[head_len + insert_len + tail_len] = '\0';

    return new_text;
}


bool check_version_compatibility(const char* str) {
    if (str == NULL) {
        return false;
    }

    const char* line_start = str;

    while (*line_start) {
        const char* line_end = strpbrk(line_start, "\n");
        size_t line_len;

        if (line_end) {
            line_len = line_end - line_start;
        } else {
            line_len = strlen(line_start);
        }

        if (strncmp(line_start, "#version", 8) == 0) {
            char current_line[line_len + 1];
            strncpy(current_line, line_start, line_len);
            current_line[line_len] = '\0';

            if (strstr(current_line, "compatibility") != NULL) {
                return true;
            }
        }

        if (line_end) {
            line_start = line_end + 1;
        } else {
            break;
        }
    }

    return false;
}

void remove_before_version(char* str) {
    if (str == NULL) {
        return;
    }

    char* version_ptr = strstr(str, "#version");

    if (version_ptr != NULL) {
        size_t remaining_len = strlen(version_ptr) + 1;

        memmove(str, version_ptr, remaining_len);
    }
}

int has_gl_FragColor_definition(const char* s) {
    const char* p = s;
    while (*p) {
        if (strncmp(p, "vec4", 4) == 0) {
            p += 4;
            while (*p && isspace((unsigned char)*p))
                p++;
            if (strncmp(p, "gl4es_FragColor", 12) == 0) {
                return 1;
            }
        } else {
            p++;
        }
    }
    return 0;
}

char* replace_glFragColor(const char* src) {
    if (!src) return NULL;

    const char* target = "gl_FragColor";
    const char* replacement = "gl4es_FragColor";
    size_t target_len = strlen(target);
    size_t replacement_len = strlen(replacement);

    size_t count = 0;
    const char* p = src;
    while ((p = strstr(p, target)) != NULL) {
        if ((p == src || !((p[-1] >= 'a' && p[-1] <= 'z') || (p[-1] >= 'A' && p[-1] <= 'Z') ||
                           (p[-1] >= '0' && p[-1] <= '9') || p[-1] == '_')) &&
            !((p[target_len] >= 'a' && p[target_len] <= 'z') || (p[target_len] >= 'A' && p[target_len] <= 'Z') ||
              (p[target_len] >= '0' && p[target_len] <= '9') || p[target_len] == '_')) {
            count++;
        }
        p += target_len;
    }

    size_t new_len = strlen(src) + count * (replacement_len - target_len) + 1;
    char* result = (char*)malloc(new_len);
    if (!result) return NULL;

    const char* src_p = src;
    char* dst_p = result;
    while ((p = strstr(src_p, target)) != NULL) {
        int valid =
            (p == src_p || !((p[-1] >= 'a' && p[-1] <= 'z') || (p[-1] >= 'A' && p[-1] <= 'Z') ||
                             (p[-1] >= '0' && p[-1] <= '9') || p[-1] == '_')) &&
            !((p[target_len] >= 'a' && p[target_len] <= 'z') || (p[target_len] >= 'A' && p[target_len] <= 'Z') ||
              (p[target_len] >= '0' && p[target_len] <= '9') || p[target_len] == '_');
        size_t copy_len = p - src_p;
        memcpy(dst_p, src_p, copy_len);
        dst_p += copy_len;

        if (valid) {
            memcpy(dst_p, replacement, replacement_len);
            dst_p += replacement_len;
        } else {
            memcpy(dst_p, target, target_len);
            dst_p += target_len;
        }
        src_p = p + target_len;
    }
    strcpy(dst_p, src_p);

    return result;
}

char* insert_gl_FragColor_if_missing(const char* shader) {
    if (has_gl_FragColor_definition(shader)) {
        return strdup(shader);
    }

    const char* insert_line = "out vec4 gl4es_FragColor;\n";

    const char* pos = shader;
    const char* insert_pos = shader;
    if (strncmp(shader, "#version", 8) == 0) {
        const char* newline = strchr(shader, '\n');
        if (newline) {
            insert_pos = newline + 1;
        }
    }

    size_t new_len = strlen(shader) + strlen(insert_line) + 1;
    char* new_shader = (char*)malloc(new_len);
    if (!new_shader) return NULL;

    size_t head_len = insert_pos - shader;
    memcpy(new_shader, shader, head_len);
    strcpy(new_shader + head_len, insert_line);
    strcpy(new_shader + head_len + strlen(insert_line), insert_pos);

    return new_shader;
}

int contains_glFragColor(const char* src) {
    if (!src) return 0;

    const char* target = "gl_FragColor";
    size_t target_len = strlen(target);

    const char* p = src;
    while ((p = strstr(p, target)) != NULL) {
        int valid = 1;

        if (p != src && (isalnum((unsigned char)p[-1]) || p[-1] == '_')) {
            valid = 0;
        }
        if (p[target_len] != '\0' && (isalnum((unsigned char)p[target_len]) || p[target_len] == '_')) {
            valid = 0;
        }

        if (valid) return 1;

        p += target_len;
    }

    return 0;
}

char* replace_fragdata0(const char* source) {
    const char* target = "gl_FragData[0].";
    const char* replacement = "vgpu_FragData0";
    size_t target_len = strlen(target);
    size_t replacement_len = strlen(replacement);

    size_t count = 0;
    const char* p = source;
    while ((p = strstr(p, target)) != NULL) {
        count++;
        p += target_len;
    }

    size_t new_len = strlen(source) + count * (replacement_len - target_len);
    char* result = (char*)malloc(new_len + 1);
    if (!result) return NULL;

    const char* src = source;
    char* dst = result;
    while ((p = strstr(src, target)) != NULL) {
        size_t len = p - src;
        memcpy(dst, src, len);
        dst += len;
        memcpy(dst, replacement, replacement_len);
        dst += replacement_len;
        src = p + target_len;
    }
    strcpy(dst, src);

    return result;
}

#define GLSL_CONVERTED_MARK "//__glsl_builtin_converted__\n"

char* mark_glsl_builtin_converted(const char* source) {
    if (!source) return NULL;
    size_t srcLen = strlen(source);
    size_t markLen = strlen(GLSL_CONVERTED_MARK);

    char* result = (char*)malloc(srcLen + markLen + 2);
    if (!result) return NULL;

    memcpy(result, source, srcLen);
    result[srcLen] = '\n';
    memcpy(result + srcLen + 1, GLSL_CONVERTED_MARK, markLen);
    result[srcLen + 1 + markLen] = '\0';

    return result;
}

int is_glsl_builtin_converted(const char* source) {
    if (!source) return 0;
    size_t srcLen = strlen(source);
    size_t markLen = strlen(GLSL_CONVERTED_MARK);

    if (srcLen < markLen) return 0;

    const char* tail = source + srcLen - markLen;
    return (strcmp(tail, GLSL_CONVERTED_MARK) == 0);
}

int first_three_lines_contains_BSL(const char* text) {
    if (!text) return 0;

    int line_count = 0;
    const char* line_start = text;
    const char* p = text;

    while (*p && line_count < 3) {
        if (*p == '\n' || *(p + 1) == '\0') {
            size_t len = p - line_start + (*(p + 1) == '\0' ? 1 : 0);

            if (len > 0) {
                if (memmem(line_start, len, "BSL", 3) != NULL) {
                    return 1;
                }
            }

            line_count++;
            line_start = p + 1;
        }
        p++;
    }
    return 0;
}

static char* replace_all(const char* src, const char* old, const char* repl) {
    if (!src || !old || !repl) return NULL;
    size_t src_len = strlen(src);
    size_t old_len = strlen(old);
    if (old_len == 0) return NULL;
    size_t repl_len = strlen(repl);

    size_t count = 0;
    const char* p = src;
    while ((p = strstr(p, old)) != NULL) {
        count++;
        p += old_len;
    }
    if (count == 0) {
        char* out = (char*)malloc(src_len + 1);
        if (out) memcpy(out, src, src_len + 1);
        return out;
    }

    size_t new_len = src_len + count * (repl_len - old_len);
    char* out = (char*)malloc(new_len + 1);
    if (!out) return NULL;

    const char* cur = src;
    char* dst = out;
    while ((p = strstr(cur, old)) != NULL) {
        size_t chunk = p - cur;
        memcpy(dst, cur, chunk);
        dst += chunk;
        memcpy(dst, repl, repl_len);
        dst += repl_len;
        cur = p + old_len;
    }
    size_t tail = strlen(cur);
    memcpy(dst, cur, tail);
    dst += tail;
    *dst = '\0';
    return out;
}

static char* insert_after_first(const char* src, const char* anchor, const char* insert_str) {
    if (!src || !anchor || !insert_str) return NULL;
    const char* p = strstr(src, anchor);
    if (!p) {
        char* out = (char*)malloc(strlen(src) + 1);
        if (out) memcpy(out, src, strlen(src) + 1);
        return out;
    }
    size_t prefix_len = (p - src) + strlen(anchor);
    size_t insert_len = strlen(insert_str);
    size_t src_len = strlen(src);
    size_t new_len = src_len + insert_len;
    char* out = (char*)malloc(new_len + 1);
    if (!out) return NULL;

    memcpy(out, src, prefix_len);
    memcpy(out + prefix_len, insert_str, insert_len);
    memcpy(out + prefix_len + insert_len, src + prefix_len, src_len - prefix_len);
    out[new_len] = '\0';
    return out;
}

char* bsl_patch(const char* glsl) {
    if (!glsl) return NULL;

    const char* old1 = "cloudBlendOpacity = step(viewLength, cloudViewLength);";
    const char* new1 = "cloudBlendOpacity = 0.7;";
    const char* old2 = "vgpu_FragData1 = vec4(temporalData, temporalColor);";
    const char* new2 = "vgpu_FragData1 = vec4(color, 1.0);";
    const char* old3 = "vgpu_FragData1 = vec4(vl, 1.0);";
    const char* new3 = "vgpu_FragData1 = color;";
    const char* old4 = "highp vec3 bloom = texture(colortex1, ((coord / vec2(scale)) + offset) * resScale).xyz;";
    const char* new4 = "highp vec3 bloom = vec3(0.0, 0.0, 0.0);";
    const char* old5 = "highp float isGlowing = texture(colortex3, texCoord).z;";
    const char* new5 = "highp float isGlowing = 0.0;";
    char* s = replace_all(glsl, old1, new1);
    if (!s) return NULL;
    char* s2 = replace_all(s, old2, new2);
    free(s);
    if (!s2) return NULL;
    char* s3 = replace_all(s2, old3, new3);
    free(s2);
    if (!s3) return NULL;
    char* s4 = replace_all(s3, old4, new4);
    free(s3);
    if (!s4) return NULL;
    char* s5 = replace_all(s4, old5, new5);
    free(s4);
    if (!s5) return NULL;

    const char* trigger = "highp float quality[";
    char* cur = s5;

    if (strstr(cur, trigger) != NULL) {
        const char* anchor0 = "layout(location = 0) out highp vec4 vgpu_FragData0;";
        const char* line0 = "\nlayout(location = 1) out highp vec4 vgpu_FragData1;";
        char* t = insert_after_first(cur, anchor0, line0);
        free(cur);
        cur = t ? t : strdup("");
        const char* anchor1 = "vgpu_FragData0 = vec4(color, 1.0);";
        const char* line1 = "\nvgpu_FragData1 = vec4(color,1.0);";
        t = insert_after_first(cur, anchor1, line1);
        free(cur);
        cur = t ? t : strdup("");
    }
    return cur;
}

void APIENTRY_GL4ES gl4es_glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string,
                                         const GLint* length) {
    DBG(SHUT_LOGD("glShaderSource(%d, %d, %p, %p)\n", shader, count, string, length))
    // sanity check
    if (count <= 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    CHECK_SHADER(void, shader)
    // get the size of the shader sources and than concatenate in a single string
    int l = 0;
    for (int i = 0; i < count; i++)
        l += (length && length[i] >= 0) ? length[i] : strlen(string[i]);
    if (glshader->source) free(glshader->source);
    glshader->source = malloc(l + 1);
    memset(glshader->source, 0, l + 1);
    if (length) {
        for (int i = 0; i < count; i++) {
            if (length[i] >= 0)
                strncat(glshader->source, string[i], length[i]);
            else
                strcat(glshader->source, string[i]);
        }
    } else {
        for (int i = 0; i < count; i++)
            strcat(glshader->source, string[i]);
    }
    LOAD_GLES2(glShaderSource);
    if (gles_glShaderSource) {
        // adapt shader if needed (i.e. not an es2 context and shader is not #version 100)
        if (is_direct_shader(glshader->source)) {
            glshader->converted = strdup(glshader->source);
        } else {
            int glsl_version = getGLSLVersion(glshader->source);
            DBG(SHUT_LOGD("[INFO] [Shader] Shader source: "))
            DBG(SHUT_LOGD("%s", glshader->source))
            int isFPEShader = (strstr(glshader->source, fpeshader_signature) != NULL) ? 1 : 0;
            if (glsl_version < 140 && !isFPEShader) {
                glshader->source = replace_version_line(glshader->source);
                glsl_version = 460;
            }
            if (glsl_version < 140 || globals4es.esversion < 300) {
                glshader->converted = strdup(ConvertShaderConditionally(glshader));
                glshader->is_converted_essl_320 = 0;
            } else {
                if (check_version_compatibility(glshader->source)) {
                    // bool isBSL = first_three_lines_contains_BSL(glshader->source);
                    bool isBuiltInVariableConverted = is_glsl_builtin_converted(glshader->source);
                    remove_before_version(glshader->source);
                    char* convertedSource =
                        ConvertShaderBuiltInVariableOnly(glshader->source, glshader->type == GL_VERTEX_SHADER ? 1 : 0,
                                                         &glshader->need, isBuiltInVariableConverted ? 0 : 1);
                    free(glshader->source);
                    if (glshader->type == GL_FRAGMENT_SHADER) {
                        if (contains_glFragColor(glshader->source)) {
                            convertedSource = replace_glFragColor(convertedSource);
                            convertedSource = insert_gl_FragColor_if_missing(convertedSource);
                        } else {
                            int sourceLength = strlen(convertedSource) + 1;
                            convertedSource = ReplaceGLFragData(convertedSource, &sourceLength);
                        }
                    }
                    if (!isBuiltInVariableConverted) convertedSource = mark_glsl_builtin_converted(convertedSource);
                    glshader->source = strdup(convertedSource);
                    convertedSource = replace_version_line_460(convertedSource);
                    int returnCode = 0; // TODO: handle returnCode
                    char* result = GLSLtoGLSLES_c(convertedSource, glshader->type, globals4es.esversion, glsl_version,
                                                  &returnCode);
                    free(convertedSource);
                    glshader->converted =
                        strdup(result != NULL ? process_uniform_declarations(result, glshader->uniforms_declarations,
                                                                             &glshader->uniforms_declarations_count)
                                              : ConvertShaderConditionally(glshader));
                    glshader->converted = process_uniform_declarations(
                        glshader->converted, glshader->uniforms_declarations, &glshader->uniforms_declarations_count);

                    /*if (isBSL)*/ glshader->converted = bsl_patch(glshader->converted);
                    glshader->is_converted_essl_320 = 0;
                } else {
                    int returnCode = 0; // TODO: handle returnCode
                    char* result = GLSLtoGLSLES_c(glshader->source, glshader->type, globals4es.esversion, glsl_version,
                                                  &returnCode);
                    glshader->converted =
                        strdup(result != NULL ? process_uniform_declarations(result, glshader->uniforms_declarations,
                                                                             &glshader->uniforms_declarations_count)
                                              : ConvertShaderConditionally(glshader));
                    glshader->is_converted_essl_320 = 1;
                }
            }

            DBG(SHUT_LOGD("\n[INFO] [Shader] Converted Shader source: \n%s", glshader->converted))


            // ===================================================================
            // === BEGIN ULTIMATE FIXUP PATCH v10 (The Final Stroke)
            // ===================================================================
            if (glshader->converted && (glshader->type == GL_VERTEX_SHADER || glshader->type == GL_FRAGMENT_SHADER)) {
                //SHUT_LOGD("LIBGL: Executing ULTIMATE FIXUP PATCH v10 on Shader ID %d (Type: %s)\n", glshader->id, (glshader->type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment");

                // Part 1: Force the correct GLSL ES version for BOTH shaders
                char target_version[32];
                if (globals4es.esversion >= 320) sprintf(target_version, "#version 320 es");
                else if (globals4es.esversion >= 310) sprintf(target_version, "#version 310 es");
                else sprintf(target_version, "#version 300 es");

                char* temp_v = force_replace_version(glshader->converted, target_version);
                if (temp_v) { free(glshader->converted); glshader->converted = temp_v; }

                // Part 2: Replace old texture2D with modern texture() for BOTH shaders
                if (strstr(glshader->converted, "texture2D")) {
                    char* temp_tex = replace_all(glshader->converted, "texture2D", "texture");
                    if (temp_tex) { free(glshader->converted); glshader->converted = temp_tex; }
                }

                // Part 3: Handle keywords based on shader type
                if (glshader->type == GL_VERTEX_SHADER) {
                    // In Vertex Shaders, 'attribute' becomes 'in', and 'varying' becomes 'out'
                    if (strstr(glshader->converted, "attribute")) {
                        char* temp_ain = replace_all(glshader->converted, "attribute", "in");
                        if (temp_ain) { free(glshader->converted); glshader->converted = temp_ain; }
                    }
                    if (strstr(glshader->converted, "varying")) {
                        char* temp_vout = replace_all(glshader->converted, "varying", "out");
                        if (temp_vout) { free(glshader->converted); glshader->converted = temp_vout; }
                    }
                } else { // This is a GL_FRAGMENT_SHADER
                    // Part 3.5: Add the mandatory default precision for floats in Fragment Shaders
                    if (!strstr(glshader->converted, "precision highp float")) {
                        const char* precision_qualifier = "\nprecision highp float;\n";
                        char* temp_prec = insert_after_first(glshader->converted, "\n", precision_qualifier);
                        if (temp_prec) { free(glshader->converted); glshader->converted = temp_prec; }
                    }

                    // In Fragment Shaders, 'varying' becomes 'in'
                    if (strstr(glshader->converted, "varying")) {
                        char* temp_vin = replace_all(glshader->converted, "varying", "in");
                        if (temp_vin) { free(glshader->converted); glshader->converted = temp_vin; }
                    }

                    // Part 4: Handle fragment-specific outputs (gl_FragColor/Data)
                    const char* custom_out_var = "fragColor";
                    int needs_out_declaration = 0;

                    if (strstr(glshader->converted, "gl_FragColor")) {
                        char* temp_fc = replace_all(glshader->converted, "gl_FragColor", custom_out_var);
                        if (temp_fc) { free(glshader->converted); glshader->converted = temp_fc; needs_out_declaration = 1; }
                    }
                    if (strstr(glshader->converted, "gl_FragData[0]")) {
                        char* temp_fd = replace_all(glshader->converted, "gl_FragData[0]", custom_out_var);
                        if (temp_fd) { free(glshader->converted); glshader->converted = temp_fd; needs_out_declaration = 1; }
                    }

                    // If we need to declare our variable, do it smartly with precision
                    if (needs_out_declaration && !strstr(glshader->converted, "out vec4 fragColor")) {
                        const char* declaration = "\nout highp vec4 fragColor;\n";
                        char* temp_decl = insert_after_preamble(glshader->converted, declaration);
                        if (temp_decl) { free(glshader->converted); glshader->converted = temp_decl; }
                    }
                }
            }
            // ===================================================================
            // === END ULTIMATE FIXUP PATCH v10
            // ===================================================================
        }
        // send source to GLES2 hardware if any
        gles_glShaderSource(
            shader, 1, (const GLchar* const*)((glshader->converted) ? (&glshader->converted) : (&glshader->source)),
            NULL);
        errorGL();
    } else
        noerrorShim();
}

#define SUPER()                                                                                                        \
    GO(color)                                                                                                          \
    GO(secondary)                                                                                                      \
    GO(fogcoord)                                                                                                       \
    GO(texcoord)                                                                                                       \
    GO(normalmatrix)                                                                                                   \
    GO(mvmatrix)                                                                                                       \
    GO(mvpmatrix)                                                                                                      \
    GO(notexarray)                                                                                                     \
    GO(clean)                                                                                                          \
    GO(clipvertex)                                                                                                     \
    GO2(texs)

void accumShaderNeeds(GLuint shader, shaderconv_need_t* need) {
    CHECK_SHADER(void, shader)
    if (!glshader->converted) return;
#define GO(A)                                                                                                          \
    if (need->need_##A < glshader->need.need_##A) need->need_##A = glshader->need.need_##A;
#define GO2(A) need->need_##A |= glshader->need.need_##A;
    SUPER()
#undef GO
#undef GO2
}
int isShaderCompatible(GLuint shader, shaderconv_need_t* need) {
    CHECK_SHADER(int, shader)
    if (!glshader->converted) return 0;
#define GO(A)                                                                                                          \
    if (need->need_##A > glshader->need.need_##A) return 0;
#define GO2(A)                                                                                                         \
    if (need->need_##A & glshader->need.need_##A) return 0;
    SUPER()
#undef GO
#undef GO2
    return 1;
}
#undef SUPER

void redoShader(GLuint shader, shaderconv_need_t* need) {
    LOAD_GLES2(glShaderSource);
    if (!gles_glShaderSource) return;
    CHECK_SHADER(void, shader)
    if (!glshader->converted) return;
    // test, if no changes, no need to reconvert & recompile...
    if (memcmp(&glshader->need, need, sizeof(shaderconv_need_t)) == 0) return;
    free(glshader->converted);
    memcpy(&glshader->need, need, sizeof(shaderconv_need_t));
    if (is_direct_shader(glshader->source))
        glshader->converted = strdup(glshader->source);
    else {
        int glsl_version = getGLSLVersion(glshader->source);
        DBG(SHUT_LOGD("[INFO] [Shader] Shader source: "))
        DBG(SHUT_LOGD("%s", glshader->source))
        if (glsl_version < 150 || globals4es.esversion < 300) {
            glshader->converted = strdup(ConvertShaderConditionally(glshader));
            glshader->is_converted_essl_320 = 0;
        } else {
            int returnCode = 0;
            char* result =
                GLSLtoGLSLES_c(glshader->source, glshader->type, globals4es.esversion, glsl_version, &returnCode);
            glshader->converted =
                strdup(result != NULL ? process_uniform_declarations(result, glshader->uniforms_declarations,
                                                                     &glshader->uniforms_declarations_count)
                                      : ConvertShaderConditionally(glshader));
            glshader->is_converted_essl_320 = 1;
        }
        DBG(SHUT_LOGD("\n[INFO] [Shader] Converted Shader source: \n%s", glshader->converted))
    }
    // send source to GLES2 hardware if any
    gles_glShaderSource(
        shader, 1, (const GLchar* const*)((glshader->converted) ? (&glshader->converted) : (&glshader->source)), NULL);
    // recompile...
    gl4es_glCompileShader(glshader->id);
}

void APIENTRY_GL4ES gl4es_glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source) {
    DBG(SHUT_LOGD("glGetShaderSource(%d, %d, %p, %p)\n", shader, bufSize, length, source))
    // find shader
    CHECK_SHADER(void, shader)
    if (bufSize <= 0) {
        errorShim(GL_INVALID_OPERATION);
        return;
    }
    // if no source, then it's an empty string
    if (glshader->source == NULL) {
        noerrorShim();
        if (length) *length = 0;
        source[0] = '\0';
        return;
    }
    // copy concatenated sources
    GLsizei size = strlen(glshader->source);
    if (size + 1 > bufSize) size = bufSize - 1;
    strncpy(source, glshader->source, size);
    source[size] = '\0';
    if (length) *length = size;
    noerrorShim();
}

GLboolean APIENTRY_GL4ES gl4es_glIsShader(GLuint shader) {
    DBG(SHUT_LOGD("glIsShader(%d)\n", shader))
    // find shader
    shader_t* glshader = NULL;
    khint_t k;
    {
        khash_t(shaderlist)* shaders = glstate->glsl->shaders;
        k = kh_get(shaderlist, shaders, shader);
        if (k != kh_end(shaders)) glshader = kh_value(shaders, k);
    }
    return (glshader) ? GL_TRUE : GL_FALSE;
}

shader_t* getShader(GLuint shader) {
    khint_t k;
    {
        khash_t(shaderlist)* shaders = glstate->glsl->shaders;
        k = kh_get(shaderlist, shaders, shader);
        if (k != kh_end(shaders)) return kh_value(shaders, k);
    }
    return NULL;
}

static const char* GLES_NoGLSLSupport = "No Shader support with current backend";

void APIENTRY_GL4ES gl4es_glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) {
    DBG(SHUT_LOGD("glGetShaderInfoLog(%d, %d, %p, %p)\n", shader, maxLength, length, infoLog))
    // find shader
    CHECK_SHADER(void, shader)
    //if (maxLength <= 0) {
    //    errorShim(GL_INVALID_OPERATION);
    //    return;
    //}
    LOAD_GLES2(glGetShaderInfoLog);
    if (gles_glGetShaderInfoLog) {
        gles_glGetShaderInfoLog(glshader->id, maxLength, length, infoLog);
        errorGL();
    } else {
        strncpy(infoLog, GLES_NoGLSLSupport, maxLength);
        if (length) *length = strlen(infoLog);
    }
}

void APIENTRY_GL4ES gl4es_glGetShaderiv(GLuint shader, GLenum pname, GLint* params) {
    DBG(SHUT_LOGD("glGetShaderiv(%d, %s, %p)\n", shader, PrintEnum(pname), params))
    // find shader
    CHECK_SHADER(void, shader)
    LOAD_GLES2(glGetShaderiv);
    noerrorShim();
    switch (pname) {
    case GL_SHADER_TYPE:
        *params = glshader->type;
        break;
    case GL_DELETE_STATUS:
        *params = (glshader->deleted) ? GL_TRUE : GL_FALSE;
        break;
    case GL_COMPILE_STATUS:
        if (gles_glGetShaderiv) {
            gles_glGetShaderiv(glshader->id, pname, params);
            errorGL();
        } else {
            *params = GL_FALSE; // stub, compile always fail
        }
        break;
    case GL_INFO_LOG_LENGTH:
        if (gles_glGetShaderiv) {
            gles_glGetShaderiv(glshader->id, pname, params);
            errorGL();
        } else {
            *params = strlen(GLES_NoGLSLSupport); // stub, compile always fail
        }
        break;
    case GL_SHADER_SOURCE_LENGTH:
        if (glshader->source)
            *params = strlen(glshader->source) + 1;
        else
            *params = 0;
        break;
    default:
        errorShim(GL_INVALID_ENUM);
    }
}

void APIENTRY_GL4ES gl4es_glGetShaderPrecisionFormat(GLenum shaderType, GLenum precisionType, GLint* range,
                                                     GLint* precision) {
    LOAD_GLES2(glGetShaderPrecisionFormat);
    if (gles_glGetShaderPrecisionFormat) {
        gles_glGetShaderPrecisionFormat(shaderType, precisionType, range, precision);
        errorGL();
    } else {
        errorShim(GL_INVALID_ENUM);
    }
}

void APIENTRY_GL4ES gl4es_glShaderBinary(GLsizei count, const GLuint* shaders, GLenum binaryFormat, const void* binary,
                                         GLsizei length) {
    // TODO: check consistancy of "shaders" values
    LOAD_GLES2(glShaderBinary);
    if (gles_glShaderBinary) {
        gles_glShaderBinary(count, shaders, binaryFormat, binary, length);
        errorGL();
    } else {
        errorShim(GL_INVALID_ENUM);
    }
}

void APIENTRY_GL4ES gl4es_glReleaseShaderCompiler(void) {
    LOAD_GLES2(glReleaseShaderCompiler);
    if (gles_glReleaseShaderCompiler) {
        gles_glReleaseShaderCompiler();
        errorGL();
    } else
        noerrorShim();
}

// ========== GL_ARB_shader_objects ==============

AliasExport(GLuint, glCreateShader, , (GLenum shaderType));
AliasExport(void, glDeleteShader, , (GLuint shader));
AliasExport(void, glCompileShader, , (GLuint shader));
AliasExport(void, glShaderSource, , (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length));
AliasExport(void, glGetShaderSource, , (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source));
AliasExport(GLboolean, glIsShader, , (GLuint shader));
AliasExport(void, glGetShaderInfoLog, , (GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog));
AliasExport(void, glGetShaderiv, , (GLuint shader, GLenum pname, GLint* params));
AliasExport(void, glGetShaderPrecisionFormat, ,
            (GLenum shaderType, GLenum precisionType, GLint* range, GLint* precision));
AliasExport(void, glShaderBinary, ,
            (GLsizei count, const GLuint* shaders, GLenum binaryFormat, const void* binary, GLsizei length));
AliasExport_V(void, glReleaseShaderCompiler);

GLhandleARB APIENTRY_GL4ES gl4es_glCreateShaderObject(GLenum shaderType) {
    return gl4es_glCreateShader(shaderType);
}

AliasExport(GLhandleARB, glCreateShaderObject, ARB, (GLenum shaderType));
AliasExport(GLvoid, glShaderSource, ARB,
            (GLhandleARB shaderObj, GLsizei count, const GLcharARB** string, const GLint* length));
AliasExport(GLvoid, glCompileShader, ARB, (GLhandleARB shaderObj));
AliasExport(GLvoid, glGetShaderSource, ARB, (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB* source));
