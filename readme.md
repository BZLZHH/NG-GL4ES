# Next Generation GL4ES

**Warning: This project is still under development and contain lots of issues.**

## Project Overview

Next Generation GL4ES is an Android `so` library project specifically for the `arm64-v8a` architecture. This project enables OpenGL 3.0+ function emulation through OpenGL ES 3.0, allowing higher-level OpenGL functionality to render on Android devices without requiring native OpenGL 3.0+ API access.

## Technical Implementation

1. **API Mapping**: The project translates OpenGL 3.0+ functions into equivalent operations that are executable within an OpenGL ES 3.0 environment. 

2. **GL4ES Rendering**: This project relies on the GL4ES library, which serves as a rendering bridge for OpenGL ES. GL4ES translates OpenGL calls into OpenGL ES-compatible operations, allowing simulated OpenGL 3.0+ functions and existing OpenGL ES 3.0 functions to be rendered in a near-native manner.

## Third-Party Library: GL4ES

Next Generation GL4ES is powered by [Holy-GL4ES](https://github.com/FCL-Team/Holy-GL4ES) .