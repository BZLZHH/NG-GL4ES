#include <GL/gl.h>
#include "vertexattrib.h"

#include "../glx/hardext.h"
#include "buffers.h"
#include "enum_info.h"
#include "gl4es.h"
#include "glstate.h"

//#define DEBUG
#ifdef DEBUG
#define DBG(a) a
#else
#define DBG(a)
#endif


void __attribute__((visibility("default"))) glVertexAttribL1d(GLuint index, GLdouble x) {
    DBG(printf("glVertexAttribL1d(%d, %f)\n", index, x);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat v[4] = { (GLfloat)x, 0.0f, 0.0f, 0.0f };
    if (memcmp(glstate->vavalue[index], v, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], v, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glVertexAttribL2d(GLuint index, GLdouble x, GLdouble y) {
    DBG(printf("glVertexAttribL2d(%d, %f, %f)\n", index, x, y);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat v[4] = { (GLfloat)x, (GLfloat)y, 0.0f, 0.0f };
    if (memcmp(glstate->vavalue[index], v, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], v, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glVertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) {
    DBG(printf("glVertexAttribL3d(%d, %f, %f, %f)\n", index, x, y, z);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat v[4] = { (GLfloat)x, (GLfloat)y, (GLfloat)z, 0.0f };
    if (memcmp(glstate->vavalue[index], v, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], v, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glVertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
    DBG(printf("glVertexAttribL4d(%d, %f, %f, %f, %f)\n", index, x, y, z, w);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat v[4] = { (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w };
    if (memcmp(glstate->vavalue[index], v, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], v, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glVertexAttribL1dv(GLuint index, const GLdouble* v) {
    DBG(printf("glVertexAttribL1dv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat value[4] = { (GLfloat)v[0], 0.0f, 0.0f, 0.0f };
    if (memcmp(glstate->vavalue[index], value, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], value, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glVertexAttribL2dv(GLuint index, const GLdouble* v) {
    DBG(printf("glVertexAttribL2dv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat value[4] = { (GLfloat)v[0], (GLfloat)v[1], 0.0f, 0.0f };
    if (memcmp(glstate->vavalue[index], value, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], value, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glVertexAttribL3dv(GLuint index, const GLdouble* v) {
    DBG(printf("glVertexAttribL3dv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat value[4] = { (GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], 0.0f };
    if (memcmp(glstate->vavalue[index], value, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], value, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glVertexAttribL4dv(GLuint index, const GLdouble* v) {
    DBG(printf("glVertexAttribL4dv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib || index < 0) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    GLfloat value[4] = { (GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], (GLfloat)v[3] };
    if (memcmp(glstate->vavalue[index], value, 4 * sizeof(GLfloat)) == 0) {
        noerrorShim();
        return;
    }

    memcpy(glstate->vavalue[index], value, 4 * sizeof(GLfloat));
}

void __attribute__((visibility("default"))) glBindVertexBuffers(GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizeiptr* strides) {
    glstate_t* glstate = glstate;
    glvao_t* vao = glstate->vao;

    if (!vao) {
        errorShim(GL_INVALID_OPERATION);
        return;
    }

    for (GLsizei i = 0; i < count; ++i) {
        GLuint buffer = buffers[i];
        GLintptr offset = offsets ? offsets[i] : 0;
        GLsizeiptr stride = strides ? strides[i] : 0;
        khash_t(buff)* buffers_map = glstate->buffers;
        khint_t k = kh_get(buff, buffers_map, buffer);

        if (k == kh_end(buffers_map)) {
            errorShim(GL_INVALID_OPERATION);
            return;
        }

        glbuffer_t* glbuffer = kh_value(buffers_map, k);

        vao->vertexattrib[first + i].buffer = glbuffer;
        vao->vertexattrib[first + i].pointer = (const void*)offset;
        vao->vertexattrib[first + i].stride = stride;
        vao->vertexattrib[first + i].enabled = 1;
    }
}



void __attribute__((visibility("default"))) glBindVertexBuffer(GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizeiptr stride) {
    glstate_t* glstate = glstate;

    khash_t(buff)* buffers = glstate->buffers;
    khint_t k = kh_get(buff, buffers, buffer);

    if (k == kh_end(buffers)) {
        errorShim(GL_INVALID_OPERATION);
        return;
    }

    glbuffer_t* glbuffer = kh_value(buffers, k);

    glvao_t* vao = glstate->vao;

    if (vao) {
        vao->vertexattrib[bindingIndex].buffer = glbuffer;
        vao->vertexattrib[bindingIndex].pointer = (const void*)offset;
        vao->vertexattrib[bindingIndex].stride = stride;
        vao->vertexattrib[bindingIndex].enabled = 1;
    }
    else {
        errorShim(GL_INVALID_OPERATION);
    }
}

void gl4es_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer) {
    DBG(printf("glVertexAttribPointer(%d, %d, %s, %d, %d, %p), vertex buffer = %p\n", index, size, PrintEnum(type), normalized, stride, pointer, (glstate->vao->vertex)?glstate->vao->vertex->data:0);)
    FLUSH_BEGINEND;
    // sanity test
    if(index>=hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    if(size<1 || (size>4 && size!=GL_BGRA)) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    // TODO: test Type also
    vertexattrib_t *v = &glstate->vao->vertexattrib[index];
    noerrorShim();
    if(stride==0) stride=((size==GL_BGRA)?4:size)*gl_sizeof(type);
    v->size = size;
    v->type = type;
    v->normalized = normalized;
    v->stride = stride;
    v->pointer = pointer;
    v->buffer = glstate->vao->vertex;
    if( v->buffer ) {
		v->real_buffer = v->buffer->real_buffer;
		v->real_pointer = pointer;
	} else {
        v->real_buffer = 0;
        v->real_pointer = 0;
    }
}
void gl4es_glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer) {
	DBG(printf("glVertexAttribIPointer(%d, %d, %s, %d, %p)\n", index, size, PrintEnum(type), stride, pointer));
	if(!(type == GL_BYTE || type == GL_UNSIGNED_BYTE || type == GL_SHORT || type == GL_UNSIGNED_SHORT || type == GL_INT || type == GL_UNSIGNED_INT)) {
		errorShim(GL_INVALID_ENUM);
		return;
	}
	gl4es_glVertexAttribPointer(index,size,type,GL_FALSE,stride,pointer);
}
void gl4es_glEnableVertexAttribArray(GLuint index) {
    DBG(printf("glEnableVertexAttrib(%d)\n", index);)
    FLUSH_BEGINEND;
    // sanity test
    if(index>=hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vao->vertexattrib[index].enabled = 1;
}
void gl4es_glDisableVertexAttribArray(GLuint index) {
    DBG(printf("glDisableVertexAttrib(%d)\n", index);)
    FLUSH_BEGINEND;
    // sanity test
    if(index>=hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vao->vertexattrib[index].enabled = 0;
}

// TODO: move the sending of the data to the Hardware when drawing, to cache change of state
void gl4es_glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    DBG(printf("glVertexAttrib4f(%d, %f, %f, %f, %f)\n", index, v0, v1, v2, v3);)
    FLUSH_BEGINEND;
    static GLfloat f[4];
    f[0] = v0; f[1] = v1; f[2] = v2; f[3] = v3;
    gl4es_glVertexAttrib4fv(index, f);
}
void gl4es_glVertexAttrib4fv(GLuint index, const GLfloat *v) {
    DBG(printf("glVertexAttrib4fv(%d, %p)\n", index, v);)
    FLUSH_BEGINEND;
    // sanity test
    if(index<0 || index>=hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    // test if changed
    if(memcmp(glstate->vavalue[index], v, 4*sizeof(GLfloat))==0) {
        noerrorShim();
        return;
    }
    memcpy(glstate->vavalue[index], v, 4*sizeof(GLfloat));
}

#define GetVertexAttrib(suffix, Type, factor) \
void gl4es_glGetVertexAttrib##suffix##v(GLuint index, GLenum pname, Type *params) { \
    FLUSH_BEGINEND; \
    if(index<0 || index>=hardext.maxvattrib) { \
        errorShim(GL_INVALID_VALUE); \
        return; \
    } \
    noerrorShim(); \
    switch(pname) { \
        case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: \
            *params=(glstate->vao->vertexattrib[index].buffer)?glstate->vao->vertexattrib[index].buffer->buffer:0; \
            return; \
        case GL_VERTEX_ATTRIB_ARRAY_ENABLED: \
            *params=(glstate->vao->vertexattrib[index].enabled)?1:0; \
            return; \
        case GL_VERTEX_ATTRIB_ARRAY_SIZE: \
            *params=glstate->vao->vertexattrib[index].size; \
            return; \
        case GL_VERTEX_ATTRIB_ARRAY_STRIDE: \
            *params=glstate->vao->vertexattrib[index].stride; \
            return; \
        case GL_VERTEX_ATTRIB_ARRAY_TYPE: \
            *params=glstate->vao->vertexattrib[index].type; \
            return; \
        case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED: \
            *params=glstate->vao->vertexattrib[index].normalized; \
            return; \
        case GL_CURRENT_VERTEX_ATTRIB: \
            if(glstate->vao->vertexattrib[index].normalized) \
                for (int i=0; i<4; i++) \
                    *params=glstate->vavalue[index][i]*factor; \
            else    \
                for (int i=0; i<4; i++) \
                    *params=glstate->vavalue[index][i]; \
            return; \
        case GL_VERTEX_ATTRIB_ARRAY_DIVISOR: \
            *params=glstate->vao->vertexattrib[index].divisor; \
            return; \
    } \
    errorShim(GL_INVALID_ENUM); \
}

GetVertexAttrib(d, GLdouble, 1.0);
GetVertexAttrib(f, GLfloat, 1.0f);
GetVertexAttrib(i, GLint, 2147483647.0f);
#undef GetVertexAttrib

void gl4es_glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer) {
    FLUSH_BEGINEND;
    // sanity test
    if(index<0 || index>=hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    if (pname!=GL_VERTEX_ATTRIB_ARRAY_POINTER) {
        errorShim(GL_INVALID_ENUM);
        return;
    }
    *pointer = (GLvoid*)glstate->vao->vertexattrib[index].pointer;
    noerrorShim();
}

void gl4es_glVertexAttribDivisor(GLuint index, GLuint divisor) {
    FLUSH_BEGINEND;
    // sanity test
    if(index<0 || index>=hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vao->vertexattrib[index].divisor = divisor;
}

__attribute__((visibility("default"))) void glVertexAttribI1i(GLuint index, GLint x) {
    DBG(printf("glVertexAttribI1i(%d, %d)\n", index, x);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI2i(GLuint index, GLint x, GLint y) {
    DBG(printf("glVertexAttribI2i(%d, %d, %d)\n", index, x, y);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    glstate->vavalue[index][1] = (GLfloat)y;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI3i(GLuint index, GLint x, GLint y, GLint z) {
    DBG(printf("glVertexAttribI3i(%d, %d, %d, %d)\n", index, x, y, z);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    glstate->vavalue[index][1] = (GLfloat)y;
    glstate->vavalue[index][2] = (GLfloat)z;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w) {
    DBG(printf("glVertexAttribI4i(%d, %d, %d, %d, %d)\n", index, x, y, z, w);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    glstate->vavalue[index][1] = (GLfloat)y;
    glstate->vavalue[index][2] = (GLfloat)z;
    glstate->vavalue[index][3] = (GLfloat)w;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI1ui(GLuint index, GLuint x) {
    DBG(printf("glVertexAttribI1ui(%d, %u)\n", index, x);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI2ui(GLuint index, GLuint x, GLuint y) {
    DBG(printf("glVertexAttribI2ui(%d, %u, %u)\n", index, x, y);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    glstate->vavalue[index][1] = (GLfloat)y;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z) {
    DBG(printf("glVertexAttribI3ui(%d, %u, %u, %u)\n", index, x, y, z);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    glstate->vavalue[index][1] = (GLfloat)y;
    glstate->vavalue[index][2] = (GLfloat)z;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
    DBG(printf("glVertexAttribI4ui(%d, %u, %u, %u, %u)\n", index, x, y, z, w);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)x;
    glstate->vavalue[index][1] = (GLfloat)y;
    glstate->vavalue[index][2] = (GLfloat)z;
    glstate->vavalue[index][3] = (GLfloat)w;
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI1iv(GLuint index, const GLint* v) {
    DBG(printf("glVertexAttribI1iv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI2iv(GLuint index, const GLint* v) {
    DBG(printf("glVertexAttribI2iv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    glstate->vavalue[index][1] = (GLfloat)v[1];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI3iv(GLuint index, const GLint* v) {
    DBG(printf("glVertexAttribI3iv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    glstate->vavalue[index][1] = (GLfloat)v[1];
    glstate->vavalue[index][2] = (GLfloat)v[2];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI4iv(GLuint index, const GLint* v) {
    DBG(printf("glVertexAttribI4iv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    glstate->vavalue[index][1] = (GLfloat)v[1];
    glstate->vavalue[index][2] = (GLfloat)v[2];
    glstate->vavalue[index][3] = (GLfloat)v[3];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI1uiv(GLuint index, const GLuint* v) {
    DBG(printf("glVertexAttribI1uiv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI2uiv(GLuint index, const GLuint* v) {
    DBG(printf("glVertexAttribI2uiv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    glstate->vavalue[index][1] = (GLfloat)v[1];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI3uiv(GLuint index, const GLuint* v) {
    DBG(printf("glVertexAttribI3uiv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    glstate->vavalue[index][1] = (GLfloat)v[1];
    glstate->vavalue[index][2] = (GLfloat)v[2];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribI4uiv(GLuint index, const GLuint* v) {
    DBG(printf("glVertexAttribI4uiv(%d, %p)\n", index, v);)
        FLUSH_BEGINEND;
    if (index < 0 || index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }
    glstate->vavalue[index][0] = (GLfloat)v[0];
    glstate->vavalue[index][1] = (GLfloat)v[1];
    glstate->vavalue[index][2] = (GLfloat)v[2];
    glstate->vavalue[index][3] = (GLfloat)v[3];
    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    DBG(printf("glVertexAttribP1ui(%d, %s, %d, %u)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)(uintptr_t)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION);
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    DBG(printf("glVertexAttribP2ui(%d, %s, %d, %u)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)(uintptr_t)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION); 
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    DBG(printf("glVertexAttribP3ui(%d, %s, %d, %u)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)(uintptr_t)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION);
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    DBG(printf("glVertexAttribP4ui(%d, %s, %d, %u)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)(uintptr_t)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION); 
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value) {
    DBG(printf("glVertexAttribP1uiv(%d, %s, %d, %p)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION);
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value) {
    DBG(printf("glVertexAttribP2uiv(%d, %s, %d, %p)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION);
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value) {
    DBG(printf("glVertexAttribP3uiv(%d, %s, %d, %p)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION);
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value) {
    DBG(printf("glVertexAttribP4uiv(%d, %s, %d, %p)\n", index, PrintEnum(type), normalized, value);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    if (glstate->vao) {
        glstate->vao->vertexattrib[index].type = type;
        glstate->vao->vertexattrib[index].enabled = 1;
        glstate->vao->vertexattrib[index].normalized = normalized;
        glstate->vao->vertexattrib[index].pointer = (const GLvoid*)value;
    }
    else {
        errorShim(GL_INVALID_OPERATION); 
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer) {
    DBG(printf("glVertexAttribLPointer(%d, %d, %s, %d, %p)\n", index, size, PrintEnum(type), stride, pointer);)
        FLUSH_BEGINEND;

    if (index >= hardext.maxvattrib) {
        errorShim(GL_INVALID_VALUE);
        return;
    }

    vertexattrib_t* attrib = &glstate->vao->vertexattrib[index];
    attrib->size = size;
    attrib->type = type;
    attrib->stride = stride;
    attrib->pointer = pointer;
    attrib->enabled = 1;
    attrib->normalized = 0;

    noerrorShim();
}

__attribute__((visibility("default"))) void glGetVertexAttribLdv(GLuint index, GLenum pname, GLdouble* params) {
    DBG(printf("glGetVertexAttribLdv(%d, %s, %p)\n", index, PrintEnum(pname), params);)

        if (index >= hardext.maxvattrib) {
            errorShim(GL_INVALID_VALUE);
            return;
        }

    vertexattrib_t* attrib = &glstate->vao->vertexattrib[index];

    switch (pname) {
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:
        *params = (GLdouble)attrib->size;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
        *params = (GLdouble)attrib->stride;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:
        *params = (GLdouble)attrib->type;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_POINTER:
        *params = (GLdouble)(uintptr_t)attrib->pointer;
        break;
    default:
        errorShim(GL_INVALID_ENUM);
        return;
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glGetVertexAttribIiv(GLuint index, GLenum pname, GLint* params) {
    DBG(printf("glGetVertexAttribIiv(%d, %s, %p)\n", index, PrintEnum(pname), params);)

        if (index >= hardext.maxvattrib) {
            errorShim(GL_INVALID_VALUE);
            return;
        }

    vertexattrib_t* attrib = &glstate->vao->vertexattrib[index];

    switch (pname) {
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:
        *params = (GLint)attrib->size;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
        *params = (GLint)attrib->stride;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:
        *params = (GLint)attrib->type;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_POINTER:
        *params = (GLint)(uintptr_t)attrib->pointer;
        break;
    default:
        errorShim(GL_INVALID_ENUM);
        return;
    }

    noerrorShim();
}

__attribute__((visibility("default"))) void glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params) {
    DBG(printf("glGetVertexAttribIuiv(%d, %s, %p)\n", index, PrintEnum(pname), params);)

        if (index >= hardext.maxvattrib) {
            errorShim(GL_INVALID_VALUE);
            return;
        }

    vertexattrib_t* attrib = &glstate->vao->vertexattrib[index];

    switch (pname) {
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:
        *params = (GLuint)attrib->size;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
        *params = (GLuint)attrib->stride;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:
        *params = (GLuint)attrib->type;
        break;
    case GL_VERTEX_ATTRIB_ARRAY_POINTER:
        *params = (GLuint)(uintptr_t)attrib->pointer;
        break;
    default:
        errorShim(GL_INVALID_ENUM);
        return;
    }

    noerrorShim();
}



void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer) AliasExport("gl4es_glVertexAttribPointer");
void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer) AliasExport("gl4es_glVertexAttribIPointer");
void glVertexAttribIPointerEXT(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer) AliasExport("gl4es_glVertexAttribIPointer");
void glEnableVertexAttribArray(GLuint index) AliasExport("gl4es_glEnableVertexAttribArray");
void glDisableVertexAttribArray(GLuint index) AliasExport("gl4es_glDisableVertexAttribArray");
void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) AliasExport("gl4es_glVertexAttrib4f");
void glVertexAttrib4fv(GLuint index, const GLfloat *v) AliasExport("gl4es_glVertexAttrib4fv");
void glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params) AliasExport("gl4es_glGetVertexAttribdv");
void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) AliasExport("gl4es_glGetVertexAttribfv");
void glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) AliasExport("gl4es_glGetVertexAttribiv");
void glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer) AliasExport("gl4es_glGetVertexAttribPointerv");

// ============= GL_ARB_vertex_shader =================
GLvoid glVertexAttrib4fARB(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) AliasExport("gl4es_glVertexAttrib4f");
GLvoid glVertexAttrib4fvARB(GLuint index, const GLfloat *v) AliasExport("gl4es_glVertexAttrib4fv");

GLvoid glVertexAttribPointerARB(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) AliasExport("gl4es_glVertexAttribPointer");

GLvoid glEnableVertexAttribArrayARB(GLuint index) AliasExport("gl4es_glEnableVertexAttribArray");
GLvoid glDisableVertexAttribArrayARB(GLuint index) AliasExport("gl4es_glDisableVertexAttribArray");

GLvoid glGetVertexAttribdvARB(GLuint index, GLenum pname, GLdouble *params) AliasExport("gl4es_glGetVertexAttribdv");
GLvoid glGetVertexAttribfvARB(GLuint index, GLenum pname, GLfloat *params) AliasExport("gl4es_glGetVertexAttribfv");
GLvoid glGetVertexAttribivARB(GLuint index, GLenum pname, GLint *params) AliasExport("gl4es_glGetVertexAttribiv");
GLvoid glGetVertexAttribPointervARB(GLuint index, GLenum pname, GLvoid **pointer) AliasExport("gl4es_glGetVertexAttribPointerv");

// ============== GL_ARB_instanced_arrays =================
void glVertexAttribDivisor(GLuint index, GLuint divisor) AliasExport("gl4es_glVertexAttribDivisor");
void glVertexAttribDivisorARB(GLuint index, GLuint divisor) AliasExport("gl4es_glVertexAttribDivisor");