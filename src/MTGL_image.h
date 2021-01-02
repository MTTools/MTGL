#pragma once

#include <stdint.h>

typedef struct {
    uint16_t width, height;
    uint8_t *image_data;
    uint32_t image_data_length;
    uint8_t bits_per_pixel;
} Image;

void MTGL_drawImage(int pos_x, int pos_y, const Image *image);
