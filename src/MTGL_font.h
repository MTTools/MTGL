#pragma once

#include "MTGL_core.h"

#include <stdint.h>

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t bytes_per_line;
	uint16_t height_offset;
	uint32_t unicode;
	uint8_t *data;
} Character;

typedef struct {
	uint16_t font_size;
	uint8_t bits_per_pixel;
	uint16_t character_count;
	Character **character_table;
} Font;

void MTGL_drawString(const char *ch, int pos_x, int pos_y, const Font *font);
