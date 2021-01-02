#include "MTGL_image.h"
#include "MTGL_core.h"

#ifdef __cplusplus
extern "C" {
#endif

// compression methods
#define IMAGE_COMPRESSION_NONE		0 // (uncompressed image_data)
#define IMAGE_COMPRESSION_LZ77		1 // LZ77~ compression

// used compression method
#define IMAGE_COMPRESSION_METHOD	IMAGE_COMPRESSION_LZ77

// image decompression buffer size
#define DECOMPRESSION_BUFFER_SIZE   (180 * 180)

// image decompression buffer
static uint8_t decompression_buffer[DECOMPRESSION_BUFFER_SIZE];

static int _getMask(uint8_t bpp) {
    return ((1 << bpp) - 1) << (8 - bpp);
}

static uint8_t _getPixelColor(uint8_t input, int bit_nr, uint8_t bpp) {
    const int Mask = _getMask(bpp) >> bit_nr;
    input &= Mask;
    input = input >> (8 - bit_nr - bpp);
    return input * 255 / ((1 << bpp) - 1);
}

void MTGL_drawImageBPP(int pos_x, int pos_y, int width, int height, const uint8_t *image_data, uint8_t bpp) {
    const uint8_t PixelsPerByte = 8 / bpp;
    const int DataCols = width / PixelsPerByte + ((width % PixelsPerByte) ? 1 : 0);

    for (int y = 0; y < height; y++) {
        for (int col = 0; col < DataCols; col++) {
            uint8_t byte = image_data[y * DataCols + col];
            for (int bit = 0; bit < 8; bit += bpp) {
                int x = col * PixelsPerByte + (bit / bpp);
                if (x < width) {
                    uint8_t color = _getPixelColor(byte, bit, bpp);
                    if (color) {
                        MTGL_drawPixel(pos_x + x, pos_y + y, color);
                    }
                }
            }
        }
    }
}

void MTGL_drawImage(int pos_x, int pos_y, const Image *image) {
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

    MTGL_drawImageBPP(pos_x, pos_y, image->width, image->height, image_data, image->bits_per_pixel);
}

#ifdef __cplusplus
}
#endif
