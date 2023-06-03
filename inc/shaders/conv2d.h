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
    compute_lib_image2d_destroy(&(conv2d->input_image2d));
    compute_lib_image2d_destroy(&(conv2d->output_image2d));
    compute_lib_ssbo_destroy(&(conv2d->kernel_ssbo));
    compute_lib_program_destroy(&(conv2d->program), GL_TRUE);
    free(conv2d);
}

static inline compute_lib_shaders_conv2d_t* compute_lib_shaders_conv2d_init(compute_lib_instance_t* inst, int local_size_x, int local_size_y, int image_width, int image_height, float* kernel, int kernel_length)
{
    compute_lib_shaders_conv2d_t* conv2d = (compute_lib_shaders_conv2d_t*) malloc(sizeof(compute_lib_shaders_conv2d_t));

    conv2d->input_image2d = COMPUTE_LIB_IMAGE2D_NEW("input_image2d", GL_TEXTURE0, image_width, image_height, GL_READ_ONLY, 4, GL_UNSIGNED_BYTE);
    conv2d->input_image2d.resource.value = 0;
    compute_lib_image2d_setup_format(&(conv2d->input_image2d));

    conv2d->output_image2d = COMPUTE_LIB_IMAGE2D_NEW("output_image2d", GL_TEXTURE1, image_width, image_height, GL_WRITE_ONLY, 4, GL_UNSIGNED_BYTE);
    conv2d->output_image2d.resource.value = 1;
    compute_lib_image2d_setup_format(&(conv2d->output_image2d));

    conv2d->kernel_ssbo = COMPUTE_LIB_SSBO_NEW("kernel_ssbo", GL_FLOAT, GL_STATIC_READ);
    conv2d->kernel_ssbo.resource.value = 2;

    conv2d->program = COMPUTE_LIB_PROGRAM_NEW(inst, NULL, 16, 16, 1);

    GLchar* program_layout_str = compute_lib_program_glsl_layout(&(conv2d->program));
    GLchar* input_image2d_layout_str = compute_lib_image2d_glsl_layout(&(conv2d->input_image2d));
    GLchar* output_image2d_layout_str = compute_lib_image2d_glsl_layout(&(conv2d->output_image2d));
    GLchar* kernel_ssbo_layout_str = compute_lib_ssbo_glsl_layout(&(conv2d->kernel_ssbo));
    asprintf(&(conv2d->program.source), _binary_src_shaders_conv2d_comp_start, program_layout_str, input_image2d_layout_str, output_image2d_layout_str, kernel_ssbo_layout_str);
    free(program_layout_str);
    free(input_image2d_layout_str);
    free(output_image2d_layout_str);
    free(kernel_ssbo_layout_str);

    if (compute_lib_program_init(&(conv2d->program)) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    /*if (compute_lib_resource_find(&(conv2d->program), &(conv2d->input_image2d.resource)) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }*/
    if (compute_lib_image2d_init(&(conv2d->input_image2d), 0) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    /*if (compute_lib_resource_find(&(conv2d->program), &(conv2d->output_image2d.resource)) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }*/
    if (compute_lib_image2d_init(&(conv2d->output_image2d), GL_COLOR_ATTACHMENT0) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    /*if (compute_lib_resource_find(&(conv2d->program), &(conv2d->kernel_ssbo.resource)) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }*/
    if (compute_lib_ssbo_init(&(conv2d->kernel_ssbo), kernel, kernel_length) != GL_NO_ERROR) {
        compute_lib_shaders_conv2d_destroy(conv2d);
        return NULL;
    }

    return conv2d;
}

#endif // GLES32COMPUTELIB_CONV2D_H
