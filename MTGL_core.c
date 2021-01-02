#include "MTGL_core.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

static void (*_flushBuffer)(void) = NULL;
static void (*_drawPixel)(int x, int y, uint8_t color) = NULL;

void MTGL_attatchHAL(void (*flushBufferFunction)(void),
        void (*drawPixelFunction)(int x, int y, uint8_t color)) {
    _flushBuffer = flushBufferFunction;
    _drawPixel = drawPixelFunction;
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

#ifdef __cplusplus
}
#endif
