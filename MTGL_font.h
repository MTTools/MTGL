#pragma once

#include "MTGL_core.h"

#include <stdint.h>

// compression methods
#define FONT_COMPRESSION_NONE      0 // (uncompressed image_data)
#define FONT_COMPRESSION_LZ77      1 // LZ77~ compression

// used compression method
#define FONT_COMPRESSION_METHOD    FONT_COMPRESSION_LZ77

// font character decompression buffer size
#define FONT_DECOMPRESSION_BUFFER_SIZE   (40 * 40)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TEXT_ALIGNMENT_VERTICAL_TOP     = 0,
    TEXT_ALIGNMENT_VERTICAL_MIDDLE  = 1,
    TEXT_ALIGNMENT_VERTICAL_BOTTOM  = 2,
    TEXT_ALIGNMENT_VERTICAL_JUSTIFY = 3,
} TextAlignmentVertical;

typedef enum {
    TEXT_ALIGNMENT_HORIZONTAL_LEFT    = 0,
    TEXT_ALIGNMENT_HORIZONTAL_CENTER  = 1,
    TEXT_ALIGNMENT_HORIZONTAL_RIGHT   = 2,
    TEXT_ALIGNMENT_HORIZONTAL_JUSTIFY = 3,
} TextAlignmentHorizontal;

typedef enum {
    // top
    TEXT_ALIGNMENT_LEFT_TOP        = (TEXT_ALIGNMENT_HORIZONTAL_LEFT    << 4) | TEXT_ALIGNMENT_VERTICAL_TOP, // default
    TEXT_ALIGNMENT_CENTER_TOP      = (TEXT_ALIGNMENT_HORIZONTAL_CENTER  << 4) | TEXT_ALIGNMENT_VERTICAL_TOP,
    TEXT_ALIGNMENT_RIGHT_TOP       = (TEXT_ALIGNMENT_HORIZONTAL_RIGHT   << 4) | TEXT_ALIGNMENT_VERTICAL_TOP,
    TEXT_ALIGNMENT_JUSTIFY_TOP     = (TEXT_ALIGNMENT_HORIZONTAL_JUSTIFY << 4) | TEXT_ALIGNMENT_VERTICAL_TOP,
    // middle
    TEXT_ALIGNMENT_LEFT_MIDDLE     = (TEXT_ALIGNMENT_HORIZONTAL_LEFT    << 4) | TEXT_ALIGNMENT_VERTICAL_MIDDLE,
    TEXT_ALIGNMENT_CENTER_MIDDLE   = (TEXT_ALIGNMENT_HORIZONTAL_CENTER  << 4) | TEXT_ALIGNMENT_VERTICAL_MIDDLE,
    TEXT_ALIGNMENT_RIGHT_MIDDLE    = (TEXT_ALIGNMENT_HORIZONTAL_RIGHT   << 4) | TEXT_ALIGNMENT_VERTICAL_MIDDLE,
    TEXT_ALIGNMENT_JUSTIFY_MIDDLE  = (TEXT_ALIGNMENT_HORIZONTAL_JUSTIFY << 4) | TEXT_ALIGNMENT_VERTICAL_MIDDLE,
    // bottom
    TEXT_ALIGNMENT_LEFT_BOTTOM     = (TEXT_ALIGNMENT_HORIZONTAL_LEFT    << 4) | TEXT_ALIGNMENT_VERTICAL_BOTTOM,
    TEXT_ALIGNMENT_CENTER_BOTTOM   = (TEXT_ALIGNMENT_HORIZONTAL_CENTER  << 4) | TEXT_ALIGNMENT_VERTICAL_BOTTOM,
    TEXT_ALIGNMENT_RIGHT_BOTTOM    = (TEXT_ALIGNMENT_HORIZONTAL_RIGHT   << 4) | TEXT_ALIGNMENT_VERTICAL_BOTTOM,
    TEXT_ALIGNMENT_JUSTIFY_BOTTOM  = (TEXT_ALIGNMENT_HORIZONTAL_JUSTIFY << 4) | TEXT_ALIGNMENT_VERTICAL_BOTTOM,
    // justified
    TEXT_ALIGNMENT_LEFT_JUSTIFY    = (TEXT_ALIGNMENT_HORIZONTAL_LEFT    << 4) | TEXT_ALIGNMENT_VERTICAL_JUSTIFY,
    TEXT_ALIGNMENT_CENTER_JUSTIFY  = (TEXT_ALIGNMENT_HORIZONTAL_CENTER  << 4) | TEXT_ALIGNMENT_VERTICAL_JUSTIFY,
    TEXT_ALIGNMENT_RIGHT_JUSTIFY   = (TEXT_ALIGNMENT_HORIZONTAL_RIGHT   << 4) | TEXT_ALIGNMENT_VERTICAL_JUSTIFY,
    TEXT_ALIGNMENT_JUSTIFY_JUSTIFY = (TEXT_ALIGNMENT_HORIZONTAL_JUSTIFY << 4) | TEXT_ALIGNMENT_VERTICAL_JUSTIFY,
} TextAlignment;

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

void MTGL_drawString(const char *str, int pos_x, int pos_y, const Font *font,
        float line_spacing);

void MTGL_drawStringAligned(const char *str, int pos_x, int pos_y,
        const Font *font, float line_spacing, MTGLSize area,
        TextAlignment alignment);

MTGLSize Font_getStringSize(const char *str, const Font *font, float line_spacing);

#ifdef __cplusplus
}
#endif
