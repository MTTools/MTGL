#include "MTGL_core.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static uint32_t _screen_width = 0;
static uint32_t _screen_height = 0;
static uint8_t _screen_bpp = 0;
static uint8_t *_screen_buffer = NULL;
static void (*_flushBuffer)(void) = NULL;
static uint32_t _screen_buffer_size = 0;

static uint8_t _screen_pixels_per_byte;
static uint8_t _screen_max_color;

void MTGL_attatchHAL(MTGLInitStruct *init_struct) {
    _screen_width = init_struct->screen_width;
    _screen_height = init_struct->screen_height;
    _screen_bpp = init_struct->screen_bpp;
    _flushBuffer = init_struct->flushBufferFunction;
    _screen_buffer = init_struct->screen_buffer;
    _screen_buffer_size = _screen_height * _screen_width * _screen_bpp / 8;

    _screen_pixels_per_byte = 8 / _screen_bpp;
    _screen_max_color = (1 << _screen_bpp) - 1;
}

void MTGL_flushBuffer(void) {
    if (_flushBuffer != NULL) {
        _flushBuffer();
    }
}

void MTGL_drawPixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= _screen_width || y < 0 || y >= _screen_height) {
        return;
    }

    color = color * _screen_max_color / 255;

    const int col = x / _screen_pixels_per_byte;
    const int pix = x % _screen_pixels_per_byte;
    const int shift = (1 - pix) * _screen_bpp;
    const int bits = color << shift;
    const int clear = ~(_screen_max_color << shift);
    const int buffer_pos = y * _screen_width / 2 + col;

    _screen_buffer[buffer_pos] &= clear;
    _screen_buffer[buffer_pos] |= bits;
}

void MTGL_fill(uint8_t color) {
    color = color * _screen_max_color / 255;
    for (int i = 1; i < _screen_pixels_per_byte; i++) {
        color |= color << _screen_bpp;
    }
    memset(_screen_buffer, color, _screen_buffer_size);
}

#ifdef __cplusplus
}
#endif
