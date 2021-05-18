#include "MTGL_font.h"

#include <string.h>
#include <stddef.h>

#if FONT_COMPRESSION_METHOD == FONT_COMPRESSION_LZ77
#include "LZ77/lz77.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// image decompression buffer
#if FONT_COMPRESSION_METHOD != FONT_COMPRESSION_NONE
static uint8_t decompression_buffer[FONT_DECOMPRESSION_BUFFER_SIZE];
#endif

extern void MTGL_drawImageBPPBrightness(int pos_x, int pos_y, int width, int height, const uint8_t *image_data, uint8_t bpp, uint8_t brightness);
extern void MTGL_drawImageBPP(int pos_x, int pos_y, int width, int height, const uint8_t *image_data, uint8_t bpp);

// ------------------------------ basic private font functions ------------------------------

typedef struct {
    char utf8[5];
    uint8_t utf8_len;
    uint32_t unicode;
} Utf8UnicodeChar;

static Utf8UnicodeChar _charFromUtf8(const char *chr) {
    Utf8UnicodeChar result;
    memset(&result, 0, sizeof(result));

    if ((chr[0] & 0b10000000) == 0) { // 0xxxxxxx (U+0000 - U+007F)
        result.unicode = chr[0];
        result.utf8_len = 1;
    }
    else if ((chr[0] & 0b11000000) == 0b10000000) { // 10xxxxxx (U+0080 - U+00BF)
        result.unicode = chr[0];
        result.utf8_len = 1;
    }
    else if ((chr[0] & 0b11100000) == 0b11000000) { // 110xxxxx xxxxxxxx (U+00C0 - U+1FFF)
        result.unicode = ((chr[0] & 0b00011111) << 6) | (chr[1] & 0b00111111);
        result.utf8_len = 2;
    }
    else if ((chr[0] & 0b11110000) == 0b11100000) { // 1110xxxx 10xxxxxx 10xxxxxx (U+0800 - U+FFFF)
        result.unicode = ((chr[0] & 0b00001111) << 12) | ((chr[1] & 0b00111111) << 6) | (chr[2] & 0b00111111);
        result.utf8_len = 3;
    }
    else if ((chr[0] & 0b11111000) == 0b11110000) { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx (U+00010000 - U+001FFFFF)
        result.unicode = ((chr[0] & 0b00000111) << 18) | ((chr[1] & 0b00111111) << 12) | ((chr[2] & 0b00111111) << 6) | (chr[3] & 0b00111111);
        result.utf8_len = 4;
    }
    else if ((chr[0] & 0b11111100) == 0b11111000) { // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx (U+00200000 - U+03FFFFFF)
        result.unicode = ((chr[0] & 0b00000011) << 24) | ((chr[1] & 0b00111111) << 18) | ((chr[2] & 0b00111111) << 12) | ((chr[3] & 0b00111111) << 6) | (chr[4] & 0b00111111);
        result.utf8_len = 5;
    }
    else if ((chr[0] & 0b11111110) == 0b11111100) { // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx (U+04000000 - U+7FFFFFFF)
        result.unicode = ((chr[0] & 0b00000001) << 30) | ((chr[1] & 0b00111111) << 24) | ((chr[2] & 0b00111111) << 18) | ((chr[3] & 0b00111111) << 12) | ((chr[4] & 0b00111111) << 6) | (chr[5] & 0b00111111);
        result.utf8_len = 6;
    }

    memcpy(result.utf8, chr, result.utf8_len);
    return result;
}

    // binary search
static Character * _getCharacterFromArray(uint32_t unicode_char, Character **array, uint16_t count) {
    uint32_t first = 0;
    uint32_t last = count - 1;
    uint32_t middle = (first + last) / 2;

    while (first <= last) {
        if (array[middle]->unicode < unicode_char) {
            first = middle + 1;
        }
        else if (array[middle]->unicode == unicode_char) {
            return array[middle];
        } else {
            last = middle - 1;
        }

        middle = (first + last) / 2;
    }

    return NULL;
}

static Character * Font_getCharUTF8(const Font *font, const char *utf8_char, uint8_t *out_byte_count) {
    Utf8UnicodeChar ch = _charFromUtf8(utf8_char);
    if (out_byte_count != NULL) {
        *out_byte_count = ch.utf8_len;
    }
    return _getCharacterFromArray(ch.unicode, font->character_table, font->character_count);
}

MTGLSize Font_getStringSize(const char *str, const Font *font, float line_spacing) {
    MTGLSize result = {
            .width = 0,
            .height = font->font_size * line_spacing
    };
    uint32_t current_width = 0;

    while (*str != '\0') {
        if (*str == '\n') {
            // new line
            if (current_width > result.width) {
                result.width = current_width;
            }
            current_width = 0;
            result.height += font->font_size * line_spacing;
        }
        else {
            unsigned char char_len;
            Character *ch = Font_getCharUTF8(font, str, &char_len);
            if (ch != NULL) {
                current_width += ch->width;

            }
            str += char_len;
        }
    }
    if (current_width > result.width) {
        result.width = current_width;
    }

    return result;
}

// ------------------------------ public graphic font functions ------------------------------

static void _drawCharacter(Character *character, int pos_x, int pos_y, uint8_t bpp, uint8_t brightness) {
    uint8_t *image_data = character->data;

#if FONT_COMPRESSION_METHOD == FONT_COMPRESSION_NONE
    // uncompressed
#elif FONT_COMPRESSION_METHOD == FONT_COMPRESSION_LZ77
    uint32_t size = lz77_decompress(image_data, character->data_length, decompression_buffer, sizeof(decompression_buffer));
    if (size > sizeof(decompression_buffer)) {
        // too small decompression buffer
        return;
    }
    image_data = decompression_buffer;
#endif

    MTGL_drawImageBPPBrightness(pos_x, pos_y + character->height_offset,
            character->bytes_per_line * 8 / bpp, character->height, image_data, bpp, brightness);
}

void MTGL_drawString(const char *str, int pos_x, int pos_y, StringFormat *format) {
    // get lines count
    uint16_t lines_cnt = 1;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (*ptr == '\n') {
            lines_cnt++;
        }
        ptr++;
    }

    // get text height, depending on vertical align
    float line_height = format->font->font_size * format->line_spacing;
    float default_text_height = lines_cnt * line_height;
    int32_t start_pos_y = pos_y;
    TextAlignmentVertical alignment_h = format->alignment & 0b11; // 2 lsb
    TextAlignmentHorizontal alignment_v = format->alignment >> 2; // 2 msb
    switch (alignment_v) {
    case TEXT_ALIGNMENT_VERTICAL_JUSTIFY: {
        // keep default start pos y
        // add extra spacing
        if (lines_cnt > 1 && default_text_height < format->area.height) {
            line_height += (format->area.height - default_text_height) / (lines_cnt - 1);
        }
        break;
    }
    case TEXT_ALIGNMENT_VERTICAL_MIDDLE:
        start_pos_y += (format->area.height - default_text_height) / 2;
        // keep default line height
        break;
    case TEXT_ALIGNMENT_VERTICAL_BOTTOM:
        start_pos_y += (format->area.height - default_text_height);
        // keep default line height
        break;
    default:
        // keep default line height
        break;
    }

    const char space_char[] = " ";
    uint8_t char_len;
    uint16_t default_space_char_width = format->font->font_size / 2;
    Character *space_ch = Font_getCharUTF8(format->font, space_char, &char_len);
    if (space_ch != NULL) {
        default_space_char_width = space_ch->width;
    }

    uint16_t line_nr = 0;
    while (*str != '\0') {
        const char *curr_line = str;
        // measure current line
        uint32_t space_count = 0;
        uint32_t line_width = 0;
        while ((*curr_line != '\0') && (*curr_line != '\n')) {
            Character *ch = Font_getCharUTF8(format->font, curr_line, &char_len);
            if (*curr_line == ' ') {
                space_count++;
            }
            if (NULL != ch) {
                line_width += ch->width;
            }
            curr_line += char_len;
        }

        // get space and text width
        int start_pos_x = pos_x;
        uint16_t extra_space_width = 0;
        switch (alignment_h) {
        case TEXT_ALIGNMENT_HORIZONTAL_CENTER:
            start_pos_x += ((format->area.width - line_width) / 2);
            break;
        case TEXT_ALIGNMENT_HORIZONTAL_JUSTIFY:
            if (line_width < format->area.width && space_count > 0) {
                // split missing width to spaces by count
                extra_space_width = (format->area.width - line_width) / space_count;
            }
            break;
        case TEXT_ALIGNMENT_HORIZONTAL_RIGHT:
            start_pos_x += (format->area.width - line_width);
            break;
        default:
            // alignment left
            break;
        }

        // draw current line
        int curr_pos_x = start_pos_x;
        int curr_pos_y = start_pos_y + line_nr * line_height;
        while ((*str != '\0') && (*str != '\n')) {
            Character *ch = Font_getCharUTF8(format->font, str, &char_len);
            if (ch != NULL) {
                _drawCharacter(ch, curr_pos_x, curr_pos_y, format->font->bits_per_pixel, format->brightness);
                curr_pos_x += ch->width;
            }
            if (*str == ' ') {
                curr_pos_x += extra_space_width;
                if (ch == NULL) {
                    curr_pos_x += default_space_char_width;
                }
            }
            str += char_len;
        }
        if ('\n' == *str) {
            str++;
        }

        line_nr++;
    }
}

#ifdef __cplusplus
}
#endif
