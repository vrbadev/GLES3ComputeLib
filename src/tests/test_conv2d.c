/// \file test_conv2d.c
/// \author Vojtech Vrba (vrba.vojtech [at] fel.cvut.cz)
/// \date May 2023
/// \brief Test source file for GLES32ComputeLib library implementation of 2D convolution.
/// \copyright GNU Public License.

#include "shaders/conv2d.h"
#include "utils/image.h"

#define LOCAL_SIZE_X 16
#define LOCAL_SIZE_Y 16

static const float kernel[3][3] = {
    { 0, -1, 0 },
    { -1, 5, -1 },
    { 0, -1, 0 }
};
static const int kernel_length = sizeof(kernel) / sizeof(float);


int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path to input image> <path to output image>\r\n", argv[0]);
        return 1;
    }

    printf("Using 2D convolution kernel with %d elements.\r\n", kernel_length);

    rgba_t* input_img_data;
    int width, height;
    printf("Loading image: %s\r\n", argv[1]);
    if (!image_load(argv[1], &width, &height, 4, (unsigned char**) &input_img_data)) {
        fprintf(stderr, "Failed to load image file!\r\n");
        return 2;
    }
    printf("Loaded image with size %dx%d px.\r\n", width, height);

    printf("Initializing compute library instance.\r\n");
    compute_lib_instance_t inst = COMPUTE_LIB_INSTANCE_NEW("/dev/dri/renderD128");
    if (compute_lib_init(&inst) != GL_NO_ERROR) {
        compute_lib_error_queue_flush(&inst, stderr);
        return 3;
    }

    printf("Initializing conv2d instance.\r\n");
    compute_lib_shaders_conv2d_t* conv2d;
    if ((conv2d = compute_lib_shaders_conv2d_init(&inst, LOCAL_SIZE_X, LOCAL_SIZE_Y, width, height, (float*) kernel, kernel_length)) == NULL) {
        compute_lib_error_queue_flush(&inst, stderr);
        return 4;
    }

    printf("Running conv2d program.\r\n");
    if (compute_lib_image2d_write(&(conv2d->input_image2d), input_img_data) != GL_NO_ERROR) {
        compute_lib_error_queue_flush(&inst, stderr);
        return 5;
    }
    if (compute_lib_program_dispatch(&(conv2d->program), width, height, 1) != GL_NO_ERROR) {
        compute_lib_error_queue_flush(&inst, stderr);
        return 6;
    }

    rgba_t* output_img_data = (rgba_t*) calloc(width*height, sizeof(rgba_t));

    if (compute_lib_image2d_read(&(conv2d->output_image2d), output_img_data) != GL_NO_ERROR) {
        compute_lib_error_queue_flush(&inst, stderr);
        return 7;
    }

    printf("Writing output image: %s\r\n", argv[2]);
    if (!image_save(argv[2], width, height, 4, (unsigned char*) output_img_data)) {
        fprintf(stderr, "Failed to write image file!\r\n");
        return 8;
    }

    compute_lib_shaders_conv2d_destroy(conv2d);
    compute_lib_deinit(&inst);
    free(output_img_data);

    printf("Program Done.\r\n");
}
