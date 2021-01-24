#pragma once

#include <stdint.h>

// compression methods
#define IMAGE_COMPRESSION_NONE      0 // (uncompressed image_data)
#define IMAGE_COMPRESSION_LZ77      1 // LZ77~ compression

// used compression method
#define IMAGE_COMPRESSION_METHOD    IMAGE_COMPRESSION_LZ77

// image decompression buffer size
#define IMAGE_DECOMPRESSION_BUFFER_SIZE   (180 * 180)

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

#ifdef __cplusplus
}
#endif
