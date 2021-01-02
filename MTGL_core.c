#include "MTGL_core.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*flushBuffer)(void);
    void (*drawPixel)(int x, int y, uint8_t color);
} MTGL;

static MTGL mtgl = {
    flushBufferFunction = NULL,
    drawPixelFunction = NULL,
};

void MTGL_attatchHAL(void (*flushBufferFunction)(void),
        void (*drawPixelFunction)(int x, int y, uint8_t color),
        uint8_t screen_bpp) {
    mtgl.flushBuffer = flushBufferFunction;
    mtgl.drawPixel = drawPixelFunction;
}

void MTGL_flushBuffer(void) {
    if (mtgl.flushBuffer != NULL) {
        mtgl.flushBuffer();
    }
}
void MTGL_drawPixel(int x, int y, uint8_t color) {
    if (mtgl.drawPixel != NULL) {
        mtgl.drawPixel(x, y, color);
    }
}

#ifdef __cplusplus
}
#endif
