#include "hexdump.h"

void hexdump(const void *const data, const size_t sz) {
    hexdump_w(data, sz, _HEXDUMP_W, _HEXDUMP_S, _HEXDUMP_INDENT);
}

void hexdump_w(const void *data, const size_t sz, const size_t width, const size_t sep,
               const char *const indent) {
    const uint8_t *const d = data;
    const size_t rows = (sz % width == 0) ? (sz / width) : ((sz / width) + 1);
    printf("%s", indent);
    for (size_t row = 0; row < rows; row++) {
        size_t len = (rows != sz / width && row == rows - 1) ? (sz % width) : width;
        for (size_t i = 0; i < len; i++) {
            printf("%02x", d[(row * width) + i]);
            if (i != 0 && (i + 1) % sep == 0) {
                printf(" ");
            }
        }
        printf(" | ");
        for (size_t i = 0; i < len; i++) {
            printf((d[(row * width) + i] >= 0x21 && d[(row * width) + i] <= 0x7e) ? "%c"
                                                                                  : ".",
                   d[(row * width) + i]);
            if (i != 0 && (i + 1) % sep == 0) {
                printf(" ");
            }
        }
        printf("\n%s", indent);
    }
}