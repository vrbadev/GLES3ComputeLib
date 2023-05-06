/// \file image.h
/// \author Vojtech Vrba (vrba.vojtech [at] fel.cvut.cz)
/// \date May 2023
/// \brief This header file provides simple functions for saving and loading images in multiple formats based on filename extensions.
/// \copyright GNU Public License.

#ifndef GLES32COMPUTELIB_IMAGE_H
#define GLES32COMPUTELIB_IMAGE_H

#include <stdlib.h>
#include <stdbool.h>

#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"
#include "lodepng.h"


static inline int string_ends_with(const char* str, const char* suffix)
{
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    return (str_len >= suffix_len) && (0 == strcmp(str + (str_len-suffix_len), suffix));
}


static inline bool image_save(const char* filename, int width, int height, int num_components, unsigned char* data)
{
    if (string_ends_with(filename, ".jpg") || string_ends_with(filename, ".jpeg")) {
        unsigned char* data_write = data;
        bool allocated = false;
        if (num_components == 1) {
            num_components = 3;
            data_write = (unsigned char*) malloc(num_components * width * height);
            allocated = true;
            int pos;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    pos = y * width + x;
                    data_write[3 * pos] = data[pos];
                    data_write[3 * pos + 1] = data[pos];
                    data_write[3 * pos + 2] = data[pos];
                }
            }
        }
        int no_error = tje_encode_to_file(filename, width, height, num_components, data_write);
        if (allocated) {
            free(data_write);
        }
        if (no_error) return true;
    } else if (string_ends_with(filename, ".png")) {
        LodePNGColorType colortype;
        switch (num_components) {
            case 1:
                colortype = LCT_GREY;
                break;
            case 3:
                colortype = LCT_RGB;
                break;
            case 4:
                colortype = LCT_RGBA;
                break;
            default:
                return false;
        }
        unsigned error = lodepng_encode_file(filename, data, width, height, colortype, 8);
        if (!error) return true;
    }
    return false;
}


static inline bool image_load(const char* filename, int* width, int* height, int num_components, unsigned char** data)
{
    if (string_ends_with(filename, ".png")) {
        LodePNGColorType colortype;
        switch (num_components) {
            case 1:
                colortype = LCT_GREY;
                break;
            case 3:
                colortype = LCT_RGB;
                break;
            case 4:
                colortype = LCT_RGBA;
                break;
            default:
                return false;
        }
        unsigned error = lodepng_decode_file(data, (unsigned*) width, (unsigned*) height, filename, colortype, 8);
        if (!error) return true;
    }
    return false;
}

#endif // GLES32COMPUTELIB_IMAGE_H
