#pragma once

#include <stdint.h>

// compression methods
#define IMAGE_COMPRESSION_NONE      0 // (uncompressed image_data)
#define IMAGE_COMPRESSION_LZ77      1 // LZ77~ compression

// used compression method
#ifndef IMAGE_COMPRESSION_METHOD
#define IMAGE_COMPRESSION_METHOD    IMAGE_COMPRESSION_LZ77
#endif

// image decompression buffer size
#ifndef IMAGE_DECOMPRESSION_BUFFER_SIZE
#define IMAGE_DECOMPRESSION_BUFFER_SIZE   (180 * 180)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t width, height;
    uint8_t *image_data;
    uint32_t image_data_length;
    uint8_t bits_per_pixel;
} Image;

void MTGL_drawImage(int pos_x, int pos_y, const Image *image);
void MTGL_drawImageBrightness(int pos_x, int pos_y, const Image *image, int8_t brightness);

#ifdef __cplusplus
}
#endif
