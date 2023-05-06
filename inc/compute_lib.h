/// \file compute_lib.h
/// \author Vojtech Vrba (vrba.vojtech [at] fel.cvut.cz)
/// \date May 2023
/// \brief Main header file of the GLES32ComputeLib library.
/// \copyright GNU Public License.

#pragma once

#ifndef GLES3COMPUTELIB_COMPUTE_LIB_H
#define GLES3COMPUTELIB_COMPUTE_LIB_H

#include <fcntl.h> // open, O_RDWR
#include <unistd.h> // close
#include <string.h> // strstr, memcpy
#include <stdio.h> // FILE, fprintf
#include <stdlib.h> // malloc, calloc, free

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <gbm.h>

#include "gl32_utils.h"
#include "queue.h"


/// Structure of GLES32ComputeLib error instance.
typedef struct compute_lib_error_s {
    /// Incrementing error ID assigned by the library.
    GLuint err_id;
    /// Allocated string with error message.
    GLchar* message;
    /// Size of the allocated error string.
    GLint message_len;
    /// The source of error message.
    /// Possible values: GL_DEBUG_SOURCE_API, GL_DEBUG_SOURCE_WINDOW_SYSTEM_, GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DEBUG_SOURCE_THIRD_PARTY, GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_SOURCE_OTHER.
    GLenum source;
    /// The type of error message.
    /// Possible values: GL_DEBUG_TYPE_ERROR, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, GL_DEBUG_TYPE_PORTABILITY, GL_DEBUG_TYPE_PERFORMANCE, GL_DEBUG_TYPE_MARKER, GL_DEBUG_TYPE_PUSH_GROUP, GL_DEBUG_TYPE_POP_GROUP, GL_DEBUG_TYPE_OTHER.
    GLenum type;
    /// The ID of the error message assigned by OpenGL.
    GLuint id;
    /// The severity of error message.
    /// Possible values: GL_DEBUG_SEVERITY_NOTIFICATION, GL_DEBUG_SEVERITY_LOW, GL_DEBUG_SEVERITY_MEDIUM, GL_DEBUG_SEVERITY_HIGH.
    GLenum severity;
} compute_lib_error_t;

/// Structure of GLES32ComputeLib library instance.
typedef struct compute_lib_instance_s {
    /// Path to GPU device rendering infrastructure.
    /// E.g. "/dev/dri/renderD128"
    const GLchar* dri_path;
    /// Set to GL_TRUE if successfully initialized.
    GLboolean initialised;
    /// File descriptor for GPU DRI file.
    GLint fd;
    /// Pointer to a structure for initialized GBM device.
    struct gbm_device* gbm;
    /// EGL display handle, equals to EGL_NO_DISPLAY on error.
    EGLDisplay dpy;
    /// EGL context handle, equals to EGL_NO_CONTEXT on error.
    EGLContext ctx;
    /// Pointer to the last GLES32ComputeLib error instance.
    compute_lib_error_t* last_error;
    /// Total count of registered errors.
    GLuint error_total_cnt;
    /// Pointer to dynamically allocated queue of pointers to registered error instances.
    queue_t* error_queue;
    /// The verbosity of error message. Default value is GL_DEBUG_SEVERITY_LOW.
    /// Possible values: GL_DEBUG_SEVERITY_NOTIFICATION, GL_DEBUG_SEVERITY_LOW, GL_DEBUG_SEVERITY_MEDIUM, GL_DEBUG_SEVERITY_HIGH.
    GLenum verbosity;
} compute_lib_instance_t;

/// Structure of GLES32ComputeLib program instance.
typedef struct compute_lib_program_s {
    /// Pointer to the current GLES32ComputeLib library instance.
    compute_lib_instance_t* lib_inst;
    /// String containing GLSL shader program source for compilation.
    GLchar* source;
    /// Program handle assigned by OpenGL.
    GLuint handle;
    /// Shader program handle assigned by OpenGL.
    GLuint shader_handle;
} compute_lib_program_t;

/// Structure of GLES32ComputeLib framebuffer instance.
typedef struct compute_lib_framebuffer_s {
    /// Attachment point of the framebuffer.
    /// Possible values: GL_COLOR_ATTACHMENTi, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT.
    GLenum attachment;
    /// Framebuffer handle assigned by OpenGL.
    GLuint handle;
} compute_lib_framebuffer_t;

/// Structure of GLES32ComputeLib 2D image instance.
typedef struct compute_lib_image2d_s {
    /// String containing uniform name of the 2D image as appears in the shader source.
    const GLchar* uniform_name;
    /// Texture unit number.
    /// Possible values: GL_TEXTUREi with i ranging from 0 to GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS-1.
    GLenum texture;
    /// 2D image width in pixels.
    GLsizei width;
    /// 2D image height in pixels.
    GLsizei height;
    /// Internal image format to be used when performing formatted loads and stores into the image from shaders.
    /// Possible values: GL_RGBA32F, GL_RGBA16F, GL_R32F, GL_RGBA32UI, GL_RGBA16UI, GL_RGBA8UI, GL_R32UI, GL_RGBA32I, GL_RGBA16I, GL_RGBA8I, GL_R32I, GL_RGBA8, GL_RGBA8_SNORM.
    GLenum internal_format;
    /// Access type to be performed by shaders. Violations will lead to undefined results, possibly including program termination.
    /// Possible values: GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE.
    GLenum access;
    /// Wrap parameter for both of texture coordinates.
    /// Possible values: GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT, GL_CLAMP_TO_BORDER.
    GLfloat texture_wrap;
    /// Filter type for texture magnification and minifying functions.
    /// Possible values: GL_NEAREST, GL_LINEAR.
    GLfloat texture_filter;
    /// Format of the pixel data provided to the shader.
    /// Possible values: GL_RED, GL_RED_INTEGER, GL_RG, GL_RG_INTEGER, GL_RGB, GL_RGB_INTEGER, GL_RGBA, GL_RGBA_INTEGER, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL, GL_LUMINANCE_ALPHA, GL_LUMINANCE, GL_ALPHA.
    GLenum format;
    /// Data type of the pixel data.
    /// Possible values: GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_UNSIGNED_INT_5_9_9_9_REV, GL_UNSIGNED_INT_24_8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV.
    GLenum type;
    /// Image instance handle assigned by OpenGL.
    GLuint handle;
    /// Image uniform location as appears in the shader source.
    GLuint location;
    /// Total number of bytes required for allocation of the 2D image.
    GLuint data_size;
    /// Total number of bytes required to store a single pixel of the 2D image.
    GLuint px_size;
    /// GLES32ComputeLib framebuffer instance used for rendering (GPU to CPU transfers).
    compute_lib_framebuffer_t framebuffer;
} compute_lib_image2d_t;

/// Structure of GLES32ComputeLib atomic counter buffer object (ACBO) instance.
typedef struct compute_lib_acbo_s {
    /// String containing name of the ACBO as appears in the shader source.
    const GLchar* name;
    /// Base data type of the ACBO.
    /// Possible values: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_HALF_FLOAT, GL_FLOAT.
    GLenum type;
    /// Expected usage type of the ACBO.
    /// Possible values: GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY.
    GLenum usage;
    /// ACBO instance handle assigned by OpenGL.
    GLuint handle;
    /// Program resource index for the ACBO, dynamically loaded using the compiled shader program.
    GLuint index;
} compute_lib_acbo_t;

/// Structure of GLES32ComputeLib shader storage buffer object (SSBO) instance.
typedef struct compute_lib_ssbo_s {
    /// String containing name of the SSBO as appears in the shader source.
    const GLchar* name;
    /// Base data type of the SSBO.
    /// Possible values: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_HALF_FLOAT, GL_FLOAT.
    GLenum type;
    /// Expected usage type of the SSBO.
    /// Possible values: GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY.
    GLenum usage;
    /// SSBO instance handle assigned by OpenGL.
    GLuint handle;
    /// SSBO index, dynamically loaded using the compiled shader program.
    GLuint index;
    /// Layout binding of the SSBO as appears in the shader source.
    GLuint binding;
} compute_lib_ssbo_t;

/// Structure of GLES32ComputeLib uniform instance.
typedef struct compute_lib_uniform_s {
    /// String containing name of the uniform as appears in the shader source.
    const GLchar* name;
    /// Uniform location, dynamically loaded using the compiled shader program.
    GLuint location;
    /// Size of the uniform variable. Uniform variables other than arrays will have a size of 1.
    GLuint size;
    /// Base data type of the uniform.
    /// Possible values: GL_BOOL, GL_INT, GL_UNSIGNED_INT, GL_FLOAT (+ vec2, vec3, vec4, mat variants, see OpenGL docs).
    GLenum type;
    /// Uniform index, dynamically loaded using the compiled shader program.
    GLuint index;
} compute_lib_uniform_t;


/// Macro for initialization of new GLES32ComputeLib library instance.
/// \param dri_path_ Path to GPU device rendering infrastructure.
///                    E.g. "/dev/dri/renderD128"
#define COMPUTE_LIB_INSTANCE_NEW(dri_path_) ((compute_lib_instance_t) {.dri_path = (dri_path_), .initialised = false, .fd = 0, .gbm = NULL, .dpy = NULL, .ctx = EGL_NO_CONTEXT, .last_error = 0, .error_total_cnt = 0, .error_queue = NULL, .verbosity = GL_DEBUG_SEVERITY_LOW})

/// Macro for initialization of new GLES32ComputeLib program instance.
/// \param lib_inst_ Pointer to the current GLES32ComputeLib library instance.
/// \param source_ String containing GLSL shader program source for compilation.
#define COMPUTE_LIB_PROGRAM_NEW(lib_inst_, source_) ((compute_lib_program_t) {.lib_inst = (lib_inst_), .source = (source_), .handle = 0, .shader_handle = 0})

/// Macro for initialization of new GLES32ComputeLib framebuffer instance.
/// \param attachment_ Attachment point of the framebuffer.
///                      Possible values: GL_COLOR_ATTACHMENTi, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT.
#define COMPUTE_LIB_FRAMEBUFFER_NEW(attachment_) ((compute_lib_framebuffer_t) {.handle = 0, .attachment = (attachment_)})

/// Macro for initialization of new GLES32ComputeLib 2D image instance.
/// \param uniform_name_ String containing uniform name of the 2D image as appears in the shader source.
/// \param texture_ Texture unit number.
///                   Possible values: GL_TEXTUREi with i ranging from 0 to GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS-1.
/// \param width_ 2D image width in pixels.
/// \param height_ 2D image height in pixels.
/// \param internal_format_ Internal image format to be used when performing formatted loads and stores into the image from shaders.
///                           Possible values: GL_RGBA32F, GL_RGBA16F, GL_R32F, GL_RGBA32UI, GL_RGBA16UI, GL_RGBA8UI, GL_R32UI, GL_RGBA32I, GL_RGBA16I, GL_RGBA8I, GL_R32I, GL_RGBA8, GL_RGBA8_SNORM.
/// \param access_ Access type to be performed by shaders. Violations will lead to undefined results, possibly including program termination.
///                  Possible values: GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE.
/// \param texture_wrap_ Wrap parameter for both of texture coordinates.
///                        Possible values: GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT, GL_CLAMP_TO_BORDER.
/// \param texture_filter_ Filter type for texture magnification and minifying functions.
///                          Possible values: GL_NEAREST, GL_LINEAR.
/// \param format_ Format of the pixel data provided to the shader.
///                  Possible values: GL_RED, GL_RED_INTEGER, GL_RG, GL_RG_INTEGER, GL_RGB, GL_RGB_INTEGER, GL_RGBA, GL_RGBA_INTEGER, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL, GL_LUMINANCE_ALPHA, GL_LUMINANCE, GL_ALPHA.
/// \param type_ Data type of the pixel data.
///                Possible values: GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_UNSIGNED_INT_5_9_9_9_REV, GL_UNSIGNED_INT_24_8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV.
#define COMPUTE_LIB_IMAGE2D_NEW(uniform_name_, texture_, width_, height_, internal_format_, access_, texture_wrap_, texture_filter_, format_, type_) ((compute_lib_image2d_t) {.uniform_name = (uniform_name_), .texture = (texture_), .width = (width_), .height = (height_), .internal_format = (internal_format_), .access = (access_), .texture_wrap = (texture_wrap_), .texture_filter = (texture_filter_), .format = (format_), .type = (type_), .handle = 0, .location = 0, .data_size = 0, .px_size = 0, .framebuffer = COMPUTE_LIB_FRAMEBUFFER_NEW(0)})

/// Macro for initialization of new GLES32ComputeLib shader storage buffer object (SSBO) instance.
/// \param name_ String containing name of the SSBO as appears in the shader source.
/// \param type_ Base data type of the SSBO.
///                Possible values: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_HALF_FLOAT, GL_FLOAT.
/// \param usage_ Expected usage type of the SSBO.
///                 Possible values: GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY.
#define COMPUTE_LIB_SSBO_NEW(name_, type_, usage_) ((compute_lib_ssbo_t) {.name = (name_), .type = (type_), .usage = (usage_), .handle = 0, .index = 0, .binding = 0})

/// Macro for initialization of new GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param name_ String containing name of the ACBO as appears in the shader source.
/// \param type_ Base data type of the ACBO.
///                Possible values: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_HALF_FLOAT, GL_FLOAT.
/// \param usage_ Expected usage type of the ACBO.
///                 Possible values: GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY.
#define COMPUTE_LIB_ACBO_NEW(name_, type_, usage_) ((compute_lib_acbo_t) {.name = (name_), .type = (type_), .usage = (usage_), .handle = 0, .index = 0})

/// Macro for initialization of new GLES32ComputeLib uniform instance.
/// \param name_ String containing name of the uniform as appears in the shader source.
#define COMPUTE_LIB_UNIFORM_NEW(name_) ((compute_lib_uniform_t) {.name = (name_), .location = 0, .size = 0, .type = 0, .index = 0})


/// Enumeration of GLES32ComputeLib error codes.
enum compute_lib_error_e {
    COMPUTE_LIB_ERROR_NO_ERROR                          = 0,
    COMPUTE_LIB_ERROR_GROUP_INIT_FN                     = -100,
    COMPUTE_LIB_ERROR_ALREADY_INITIALISED               = -101,
    COMPUTE_LIB_ERROR_GPU_DRI_PATH                      = -102,
    COMPUTE_LIB_ERROR_CREATE_GBM_CTX                    = -103,
    COMPUTE_LIB_ERROR_EGL_PLATFORM_DISPLAY              = -104,
    COMPUTE_LIB_ERROR_EGL_INIT                          = -105,
    COMPUTE_LIB_ERROR_EGL_EXTENSION_CREATE_CTX          = -106,
    COMPUTE_LIB_ERROR_EGL_EXTENSION_KHR_CTX             = -107,
    COMPUTE_LIB_ERROR_EGL_CONFIG                        = -108,
    COMPUTE_LIB_ERROR_EGL_BIND_API                      = -109,
    COMPUTE_LIB_ERROR_EGL_CREATE_CTX                    = -110,
    COMPUTE_LIB_ERROR_EGL_MAKE_CURRENT                  = -111,
    COMPUTE_LIB_ERROR_GROUP_GL_ERROR                    = 0x0500
};


/// Initializes the library instance. Also initializes EGL and GBM.
/// \param inst Pointer to the GLES32ComputeLib library instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_init(compute_lib_instance_t* inst);

/// Deinitializes the library instance. Releases all allocated resources.
/// \param inst Pointer to the GLES32ComputeLib library instance.
void compute_lib_deinit(compute_lib_instance_t* inst);

/// Flushes the error queue to the provided output file stream.
/// \param inst Pointer to the GLES32ComputeLib library instance.
/// \param out Output file stream.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_error_queue_flush(compute_lib_instance_t* inst, FILE* out);


/// Gets number of OpenGL errors.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_gl_errors_count(void);

/// Prints the error message for provided error code to the provided output file stream.
/// \param err_code GLES32ComputeLib error code.
/// \param out Output file stream.
void compute_lib_print_error(GLint err_code, FILE* out);


/// Initializes GLES32ComputeLib program instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_program_init(compute_lib_program_t* program);

/// Prints OpenGL program log to the provided output file stream.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param out Output file stream.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_program_print_log(compute_lib_program_t* program, FILE* out);

/// Prints OpenGL shader program log to the provided output file stream.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param out Output file stream.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_program_print_shader_log(compute_lib_program_t* program, FILE* out);

/// Prints available OpenGL program resources (uniforms, SSBOs, ACBOs) to the provided output file stream.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param out Output file stream.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_program_print_resources(compute_lib_program_t* program, FILE* out);

///
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param num_groups_x Number of parallel local worker units along x-axis.
/// \param num_groups_y Number of parallel local worker units along y-axis.
/// \param num_groups_z Number of parallel local worker units along z-axis.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_program_dispatch(compute_lib_program_t* program, GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);

/// Destroys GLES32ComputeLib program instance. Releases allocated resources.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param free_source If GL_TRUE, the GLSL shader source shall be freed too.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_program_destroy(compute_lib_program_t* program, GLboolean free_source);


/// Initializes GLES32ComputeLib framebuffer instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param framebuffer Pointer to the GLES32ComputeLib framebuffer instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_framebuffer_init(compute_lib_program_t* program, compute_lib_framebuffer_t* framebuffer);

/// Destroys GLES32ComputeLib framebuffer instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param framebuffer Pointer to the GLES32ComputeLib framebuffer instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_framebuffer_destroy(compute_lib_program_t* program, compute_lib_framebuffer_t* framebuffer);


/// Initializes GLES32ComputeLib 2D image instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param image2d Pointer to the GLES32ComputeLib 2D image instance.
/// \param framebuffer_attachment Attachment point of the framebuffer. Use 0 if rendering (GPU to CPU transfer) is not planned.
///                               Possible values: GL_COLOR_ATTACHMENTi, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_image2d_init(compute_lib_program_t* program, compute_lib_image2d_t* image2d, GLenum framebuffer_attachment);

/// Destroys GLES32ComputeLib 2D image instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param image2d Pointer to the GLES32ComputeLib 2D image instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_image2d_destroy(compute_lib_program_t* program, compute_lib_image2d_t* image2d);

/// Resets 2D image to a pixel value provided.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param image2d Pointer to the GLES32ComputeLib 2D image instance.
/// \param px_data Pixel value to be used for reset. Number of available bytes must match the image format.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_image2d_reset(compute_lib_program_t* program, compute_lib_image2d_t* image2d, void* px_data);

/// Resets a patch of 2D image to a pixel value provided.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param image2d Pointer to the GLES32ComputeLib 2D image instance.
/// \param px_data Pixel value to be used for reset. Number of available bytes must match the image format.
/// \param x_min Lower bound of the x-coord interval (left patch edge).
/// \param x_max Upper bound of the x-coord interval (right patch edge).
/// \param y_min Lower bound of the y-coord interval (top patch edge).
/// \param y_max Upper bound of the y-coord interval (bottom patch edge).
/// \return Number of captured OpenGL errors.
GLuint compute_lib_image2d_reset_patch(compute_lib_program_t* program, compute_lib_image2d_t* image2d, void* px_data, GLint x_min, GLint x_max, GLint y_min, GLint y_max);

/// Writes the complete 2D image (transfers from CPU to GPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param image2d Pointer to the GLES32ComputeLib 2D image instance.
/// \param image_data Image data to be written. Number of available bytes must match the image format and image dimensions.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_image2d_write(compute_lib_program_t* program, compute_lib_image2d_t* image2d, void* image_data);

/// Renders and reads the complete 2D image (transfers from GPU to CPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param image2d Pointer to the GLES32ComputeLib 2D image instance.
/// \param image_data Image data to be read. Number of available bytes must match the image format and image dimensions.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_image2d_read(compute_lib_program_t* program, compute_lib_image2d_t* image2d, void* image_data);

/// Reads (and optionally renders at first) a patch of the 2D image (transfers from GPU to CPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param image2d Pointer to the GLES32ComputeLib 2D image instance.
/// \param image_data Image data to be read. Number of available bytes must match the image format and patch dimensions.
/// \param x_min Lower bound of the x-coord interval (left patch edge).
/// \param x_max Upper bound of the x-coord interval (right patch edge).
/// \param y_min Lower bound of the y-coord interval (top patch edge).
/// \param y_max Upper bound of the y-coord interval (bottom patch edge).
/// \param render If GL_TRUE, the 2D image will be (re-)rendered on the GPU.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_image2d_read_patch(compute_lib_program_t* program, compute_lib_image2d_t* image2d, void* image_data, GLint x_min, GLint x_max, GLint y_min, GLint y_max, GLboolean render);


/// Initializes the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param acbo Pointer to the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param data Initial data for the ACBO initialization. Use NULL to fill ACBO with zeros. Number of available bytes must match the ACBO format and length.
/// \param len Number of ACBO elements to be initialized.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_acbo_init(compute_lib_program_t* program, compute_lib_acbo_t* acbo, void* data, GLint len);

/// Destroys the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param acbo Pointer to the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_acbo_destroy(compute_lib_program_t* program, compute_lib_acbo_t* acbo);

/// Writes provided data to the ACBO instance (transfers CPU to GPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param acbo Pointer to the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param data Data to be written to the ACBO. Number of available bytes must match the ACBO format and length.
/// \param len Number of ACBO elements to be written.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_acbo_write(compute_lib_program_t* program, compute_lib_acbo_t* acbo, void* data, GLint len);

/// Writes provided unsigned integer to the ACBO instance (transfers CPU to GPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param acbo Pointer to the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param value Unsigned integer value to be written.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_acbo_write_uint_val(compute_lib_program_t* program, compute_lib_acbo_t* acbo, GLuint value);

/// Reads data from the ACBO instance (transfers GPU to CPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param acbo Pointer to the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param data Pointer to data to be read. Number of available bytes must match the ACBO format and length.
/// \param len Number of ACBO elements to be read.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_acbo_read(compute_lib_program_t* program, compute_lib_acbo_t* acbo, void* data, GLint len);

/// Reads unsigned integer from the ACBO instance (transfers GPU to CPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param acbo Pointer to the GLES32ComputeLib atomic counter buffer object (ACBO) instance.
/// \param value Pointer to the unsigned integer value to be read.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_acbo_read_uint_val(compute_lib_program_t* program, compute_lib_acbo_t* acbo, GLuint* value);


/// Initializes the GLES32ComputeLib shader storage buffer object (SSBO) instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param ssbo Pointer to the GLES32ComputeLib shader storage buffer object (SSBO) instance.
/// \param data Initial data for the SSBO initialization. Use NULL to fill SSBO with zeros. Number of available bytes must match the SSBO format and length.
/// \param len Number of SSBO elements to be initialized.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_ssbo_init(compute_lib_program_t* program, compute_lib_ssbo_t* ssbo, void* data, GLint len);

/// Destroys the GLES32ComputeLib shader storage buffer object (SSBO) instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param ssbo Pointer to the GLES32ComputeLib shader storage buffer object (SSBO) instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_ssbo_destroy(compute_lib_program_t* program, compute_lib_ssbo_t* ssbo);

/// Writes data to the SSBO instance (transfers CPU to GPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param ssbo Pointer to the GLES32ComputeLib shader storage buffer object (SSBO) instance.
/// \param data Data to be written to the SSBO. Number of available bytes must match the SSBO format and length.
/// \param len Number of SSBO elements to be written.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_ssbo_write(compute_lib_program_t* program, compute_lib_ssbo_t* ssbo, void* data, GLint len);

/// Read data from the SSBO instance (transfers GPU to CPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param ssbo Pointer to the GLES32ComputeLib shader storage buffer object (SSBO) instance.
/// \param data Data to be read from the SSBO. Number of available bytes must match the SSBO format and length.
/// \param len Number of SSBO elements to be read.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_ssbo_read(compute_lib_program_t* program, compute_lib_ssbo_t* ssbo, void* data, GLint len);


/// Initializes the GLES32ComputeLib uniform instance.
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param uniform Pointer to the GLES32ComputeLib uniform instance.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_uniform_init(compute_lib_program_t* program, compute_lib_uniform_t* uniform);

/// Writes data to the uniform instance (transfers CPU to GPU).
/// \param program Pointer to the GLES32ComputeLib program instance.
/// \param uniform Pointer to the GLES32ComputeLib uniform instance.
/// \param data Data to be written to the uniform. Number of available bytes must match the uniform format and size.
/// \return Number of captured OpenGL errors.
GLuint compute_lib_uniform_write(compute_lib_program_t* program, compute_lib_uniform_t* uniform, void* data);

#endif // GLES3COMPUTELIB_COMPUTE_LIB_H
