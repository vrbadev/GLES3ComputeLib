/// \file gl32_utils.h
/// \author Vojtech Vrba (vrba.vojtech [at] fel.cvut.cz)
/// \date May 2023
/// \brief This header file contains auxiliary functions and structs definitions extending OpenGL ES3.2.
/// \copyright GNU Public License.

#pragma once

#ifndef GLES3COMPUTELIB_GL32_UTILS_H
#define GLES3COMPUTELIB_GL32_UTILS_H

#include <GLES3/gl32.h>

/// Structure of RGBA pixel containing 8-bit unsigned channels.
typedef struct rgba_s {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} rgba_t;

/// Structure of 2D vector containing 32-bit signed components.
typedef struct ivec2_s {
    GLint x;
    GLint y;
} ivec2_t;

/// Structure of 3D vector containing 32-bit signed components.
typedef struct ivec3_s {
    GLint x;
    GLint y;
    GLint z;
} ivec3_t;

/// Structure of 4D vector containing 32-bit signed components.
typedef struct ivec4_s {
    GLint x;
    GLint y;
    GLint z;
    GLint w;
} ivec4_t;

/// Structure of 2D vector containing 32-bit unsigned components.
typedef struct uvec2_s {
    GLuint x;
    GLuint y;
} uvec2_t;

/// Structure of 3D vector containing 32-bit unsigned components.
typedef struct uvec3_s {
    GLuint x;
    GLuint y;
    GLuint z;
} uvec3_t;

/// Structure of 4D vector containing 32-bit unsigned components.
typedef struct uvec4_s {
    GLuint x;
    GLuint y;
    GLuint z;
    GLuint w;
} uvec4_t;

/// Structure of 2D vector containing 32-bit float components.
typedef struct vec2_s {
    GLfloat x;
    GLfloat y;
} vec2_t;

/// Structure of 3D vector containing 32-bit float components.
typedef struct vec3_s {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vec3_t;

/// Structure of 4D vector containing 32-bit float components.
typedef struct vec4_s {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4_t;


/// Gets name of the OpenGL defined constant by its value.
/// \param define_value Value of the OpenGL constant.
/// \return Name of the preprocessor constant.
const GLchar* gl32_get_define_name(GLuint64 define_value);

/// Gets size of OpenGL type in bytes.
/// \param type OpenGL type.
/// \return Size in bytes.
GLsizei gl32_get_type_size(GLenum type);

/// Gets number of pixel components based on the OpenGL image format.
/// \param format OpenGL image format.
/// \return Number of pixel components.
GLuint gl32_get_image_format_num_components(GLenum format);

#endif // GLES3COMPUTELIB_GL32_UTILS_H
