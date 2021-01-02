#pragma once

#include <stdint.h>

void MTGL_attatchHAL(void (*flushBufferFunction)(void), void (*drawPixelFunction)(int x, int y, uint8_t color));

void MTGL_flushBuffer(void);
void MTGL_drawPixel(int x, int y, uint8_t color);
