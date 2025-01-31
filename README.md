Krypton Wrapper
====

> [!WARNING]
> 
> The newer versions of this project WILL NOT be open-sourced for a period of time.
>
> The latest open-sourced version:
> 
> **Release 0.1.0 Preview 1 Dev-2**

> [!NOTE]
> 
> The latest version:
> 
> **Release 0.1.0**
>
> See [Releases](https://github.com/BZLZHH/NG-GL4ES/releases)

Krypton Wrapper *(also called Next Generation GL4ES/NG-GL4ES)* is a fork from [gl4es](https://github.com/ptitSeb/gl4es) and [gl4es-114-extra](https://github.com/PojavLauncherTeam/gl4es-114-extra) . 

This project is making it support more OpenGL functions.

Features
====

1. Be able to run Minecraft [Sodium](https://github.com/CaffeineMC/sodium) mod;

2. Be able to render some minecraft shaders;

3. Be able to run Minecraft in high version(like Minecraft 1.21.4 and Minecraft 1.12.2).

Usage
====
You can use the precompiled binaries of this project ANYWHERE as long as you COMPLY WITH OPEN SOURCE AGREEMENTS.

**ATTENTION**: To correctly add this renderer into your project, you should add these environment variables besides what Holy-GL4ES need:

#### For ANGLE version:

```
LIBGL_EGL=libEGL_angle.so
LIBGL_GLES=libGLESv2_angle.so
LIBGL_USE_MC_COLOR=1
```

And set ```libEGL_angle.so``` as the EGL.

#### For NO-ANGLE version:

```
LIBGL_USE_MC_COLOR=1
```

#### For them all

What's more, you can also to add ```LIBGL_FORCE_ES_COPY_TEX=1``` to improve the performance when rendering Minecraft shaders.

Change Log
====

Please see [CHANGELOG.md](https://github.com/BZLZHH/NG-GL4ES/blob/public/CHANGELOG.md)

Build
====

Please build with CMakeLists.txt.

License
====

[gl4es-114-extra](https://github.com/PojavLauncherTeam/gl4es-114-extra): MIT License

This Project (for modified code): GPL-3.0 License.

Please see [LICENSE](https://github.com/BZLZHH/NG-GL4ES/blob/public/LICENSE).


Third party components
====

**SPIRV-Cross** by **KhronosGroup**: [github](https://github.com/KhronosGroup/SPIRV-Cross)

**glslang** by **KhronosGroup**: [github](https://github.com/KhronosGroup/glslang)

**cJSON** by **DaveGamble**: [github](https://github.com/DaveGamble/cJSON)

Sponsor
====

**ptitSeb (gl4es)**: [paypal](https://paypal.me/0ptitSeb)

**PojavLauncherTeam (gl4es-114-extra)**: [patreon](https://patreon.com/pojavlauncher)
