#include "MTGL_image.h"
#include "MTGL_core.h"

#if IMAGE_COMPRESSION_METHOD == IMAGE_COMPRESSION_LZ77
#include "LZ77/lz77.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// image decompression buffer
#if IMAGE_COMPRESSION_METHOD != IMAGE_COMPRESSION_NONE
static uint8_t decompression_buffer[IMAGE_DECOMPRESSION_BUFFER_SIZE];
#endif

static int _getMask(uint8_t bpp) {
    return ((1 << bpp) - 1) << (8 - bpp);
}

static uint8_t _getPixelColor(uint8_t input, int bit_nr, uint8_t bpp) {
    const int Mask = _getMask(bpp) >> bit_nr;
    input &= Mask;
    input = input >> (8 - bit_nr - bpp);
    return input * 255 / ((1 << bpp) - 1);
}

void MTGL_drawImageBPPBrightness(int pos_x, int pos_y, int width, int height, const uint8_t *image_data, uint8_t bpp, uint8_t brightness) {
    const uint8_t PixelsPerByte = 8 / bpp;
    const int DataCols = width / PixelsPerByte + ((width % PixelsPerByte) ? 1 : 0);

    for (int y = 0; y < height; y++) {
        for (int col = 0; col < DataCols; col++) {
            uint8_t byte = image_data[y * DataCols + col];
            for (int bit = 0; bit < 8; bit += bpp) {
                int x = col * PixelsPerByte + (bit / bpp);
                if (x < width) {
                    uint8_t color = _getPixelColor(byte, bit, bpp);
                    color = color * brightness / UINT8_MAX;
                    if (color) {
                        MTGL_drawPixel(pos_x + x, pos_y + y, color);
                    }
                }
            }
        }
    }
}

void MTGL_drawImageBPP(int pos_x, int pos_y, int width, int height, const uint8_t *image_data, uint8_t bpp) {
    MTGL_drawImageBPPBrightness(pos_x, pos_y, width, height, image_data, bpp, UINT8_MAX);
}

void MTGL_drawImage(int pos_x, int pos_y, const Image *image) {
    MTGL_drawImageBrightness(pos_x, pos_y, image, INT8_MAX);
}

void MTGL_drawImageBrightness(int pos_x, int pos_y, const Image *image, int8_t brightness) {
    uint8_t *image_data = image->image_data;

#if IMAGE_COMPRESSION_METHOD == IMAGE_COMPRESSION_NONE
    // uncompressed
#elif IMAGE_COMPRESSION_METHOD == IMAGE_COMPRESSION_LZ77
    uint32_t size = lz77_decompress(image_data, image->image_data_length, decompression_buffer, sizeof(decompression_buffer));
    if (size > sizeof(decompression_buffer)) {
        // too small decompression buffer
        return;
    }
    image_data = decompression_buffer;
#endif

    MTGL_drawImageBPPBrightness(pos_x, pos_y, image->width, image->height, image_data, image->bits_per_pixel, brightness);
}

#ifdef __cplusplus
}
#endif
