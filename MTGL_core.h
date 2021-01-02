#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void MTGL_attatchHAL(int screen_width, int screen_height,
        void (*flushBufferFunction)(void),
        void (*drawPixelFunction)(int x, int y, uint8_t color),
        void (*fillFunction)(uint8_t color));

void MTGL_flushBuffer(void);
void MTGL_drawPixel(int x, int y, uint8_t color);

void MTGL_fill(uint8_t color);

#ifdef __cplusplus
}
#endif
