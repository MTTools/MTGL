#include "MTGL_core.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

static void (*_flushBuffer)(void) = NULL;
static void (*_drawPixel)(int x, int y, uint8_t color) = NULL;
static void (*_fill)(uint8_t color) = NULL;
static int _screen_width = 0;
static int _screen_height = 0;

void MTGL_attatchHAL(int screen_width, int screen_height,
        void (*flushBufferFunction)(void),
        void (*drawPixelFunction)(int x, int y, uint8_t color),
        void (*fillFunction)(uint8_t color)) {
    _screen_width = screen_width;
    _screen_height = screen_height;
    _flushBuffer = flushBufferFunction;
    _drawPixel = drawPixelFunction;
    _fill = fillFunction;
}

void MTGL_flushBuffer(void) {
    if (_flushBuffer != NULL) {
        _flushBuffer();
    }
}
void MTGL_drawPixel(int x, int y, uint8_t color) {
    if (_drawPixel != NULL) {
        _drawPixel(x, y, color);
    }
}

void MTGL_fill(uint8_t color) {
    if (_fill != NULL) {
        _fill(color);
    }
    else {
        for (int y = 0; y < _screen_height; y++) {
            for (int x = 0; x < _screen_width; x++) {
                MTGL_drawPixel(x, y, color);
            }
        }
    }
}

#ifdef __cplusplus
}
#endif
