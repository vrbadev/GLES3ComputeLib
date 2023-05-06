/// \file conv2d.h
/// \author Vojtech Vrba (vrba.vojtech [at] fel.cvut.cz)
/// \date May 2023
/// \brief Header file of the GLES32ComputeLib library implementation of 2D convolution.
/// \copyright GNU Public License.

#pragma once

#ifndef GLES32COMPUTELIB_CONV2D_H
#define GLES32COMPUTELIB_CONV2D_H

#define _GNU_SOURCE // asprintf
#include <stdio.h>

#include "compute_lib.h"

extern char _binary_src_shaders_conv2d_comp_start[];

typedef struct compute_lib_shaders_conv2d_s {
    compute_lib_program_t program;
    compute_lib_image2d_t input_image2d;
    compute_lib_image2d_t output_image2d;
    compute_lib_ssbo_t kernel_ssbo;
} compute_lib_shaders_conv2d_t;


static inline void compute_lib_shaders_conv2d_destroy(compute_lib_shaders_conv2d_t* conv2d)
{
    compute_lib_image2d_destroy(&(conv2d->program), &(conv2d->input_image2d));
    compute_lib_image2d_destroy(&(conv2d->program), &(conv2d->output_image2d));
    compute_lib_ssbo_destroy(&(conv2d->program), &(conv2d->kernel_ssbo));
    compute_lib_program_destroy(&(conv2d->program), GL_TRUE);
    free(conv2d);
}

static inline compute_lib_shaders_conv2d_t* compute_lib_shaders_conv2d_init(compute_lib_instance_t* inst, int local_size_x, int local_size_y, int image_width, int image_height, float* kernel, int kernel_length)
{
    compute_lib_shaders_conv2d_t* conv2d = (compute_lib_shaders_conv2d_t*) malloc(sizeof(compute_lib_shaders_conv2d_t));

    char* shader_src;
    asprintf(&shader_src, _binary_src_shaders_conv2d_comp_start, local_size_x, local_size_y);
    conv2d->program = COMPUTE_LIB_PROGRAM_NEW(inst, shader_src);
    if (compute_lib_program_init(&(conv2d->program)) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    conv2d->input_image2d = COMPUTE_LIB_IMAGE2D_NEW("input_image2d", GL_TEXTURE0, image_width, image_height, GL_RGBA8UI, GL_READ_ONLY, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
    if (compute_lib_image2d_init(&(conv2d->program), &(conv2d->input_image2d), 0) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    conv2d->output_image2d = COMPUTE_LIB_IMAGE2D_NEW("output_image2d", GL_TEXTURE1, image_width, image_height, GL_RGBA8UI, GL_WRITE_ONLY, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
    if (compute_lib_image2d_init(&(conv2d->program), &(conv2d->output_image2d), GL_COLOR_ATTACHMENT0) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    conv2d->kernel_ssbo = COMPUTE_LIB_SSBO_NEW("kernel_buffer", GL_FLOAT, GL_STATIC_READ);
    if (compute_lib_ssbo_init(&(conv2d->program), &(conv2d->kernel_ssbo), kernel, kernel_length) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    return conv2d;
}

#endif // GLES32COMPUTELIB_CONV2D_H
