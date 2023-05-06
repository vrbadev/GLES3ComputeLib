# GLES32ComputeLib
#### GNU C library utilising OpenGL ES3.2 offscreen rendering for image processing acceleration on GPU.
This repository contains complete CLion project for compiling static library archive and several testing executables.

The library uses OpenGL ES3.2 capabilities and currently offers:
* Library instance - used for easier handling of OpenGL features and processing of GLSL compilation errors.
* Program instances - used for compilation of the provided GLSL source. There can be multiple programs prepared to be used within the same application and with the same images and buffers.
* 2D image instances.
* Framebuffer instances (bound directly to specific 2D images for rendering).
* Shader storage buffer object (SSBO) instances.
* Atomic counter buffer object (ACBO) instances.
* Uniform variable instances.

Additionally, libraries [lodepng](https://github.com/lvandeve/lodepng) and [TinyJPEG](https://github.com/serge-rgb/TinyJPEG) are included for easier manipulation with image files.

## Requirements
CMake tries to locate `libGL.so`, `libEGL.so` and `libgbm.so` on the PC. Make sure to have these shared libraries available, they can usually be installed, e.g., by `apt` packages `libgl1-mesa-dev` and `libgbm-dev`.

## Usage
For now, there is no tutorial prepared. However, you can look into directories `src/shaders` and `inc/shaders` to see how can be simple 2D convolution implemented. To test the implementation, build and run `test_conv2d` target. 

## Licensing
The library is available under GNU General Public License v3.0.
