#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t width;
    uint32_t height;
} MTGLSize;

typedef struct {
    MTGLSize screen_size;
    uint8_t screen_bpp;
    uint8_t *screen_buffer;
    void (*flushBufferFunction)(void);
} MTGLInitStruct;

void MTGL_attatchHAL(MTGLInitStruct *init_struct);

void MTGL_flushBuffer(void);
void MTGL_drawPixel(int x, int y, uint8_t color);

void MTGL_fill(uint8_t color);

#ifdef __cplusplus
}
#endif
