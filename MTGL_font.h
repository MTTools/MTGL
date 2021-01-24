#pragma once

#include "MTGL_core.h"

#include <stdint.h>

// compression methods
#define FONT_COMPRESSION_NONE      0 // (uncompressed image_data)
#define FONT_COMPRESSION_LZ77      1 // LZ77~ compression

// used compression method
#define FONT_COMPRESSION_METHOD    FONT_COMPRESSION_LZ77

// image decompression buffer size
#define FONT_DECOMPRESSION_BUFFER_SIZE   (40 * 40)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t bytes_per_line;
	uint16_t height_offset;
	uint32_t unicode;
	uint8_t *data;
	uint32_t data_length;
} Character;

typedef struct {
	uint16_t font_size;
	uint8_t bits_per_pixel;
	uint16_t character_count;
	Character **character_table;
} Font;

void MTGL_drawString(const char *str, int pos_x, int pos_y, const Font *font, float line_spacing);

MTGLSize Font_getStringSize(const char *str, const Font *font, float line_spacing);

#ifdef __cplusplus
}
#endif
