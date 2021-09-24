#include <stdio.h>
#include <string.h>

int main() {
    char buf[0x20] = {0};
    buf[0xf] = '3';
    buf[0x0] = 'C';
    buf[0x16] = '_';
    buf[0x4] = '1';
    buf[0x1a] = '5';
    buf[0x19] = 'u';
    buf[0x9] = '3';
    buf[0xb] = 't';
    buf[0x17] = 'b';
    buf[0x11] = 't';
    buf[0x14] = 'h';
    buf[0x2] = 'C';
    buf[0x15] = '3';
    buf[0xa] = '_';
    buf[0x5] = '_';
    buf[0x13] = 't';
    buf[0x1c] = '}';
    buf[0x1] = 'C';
    buf[0x12] = '_';
    buf[0x10] = '4';
    buf[0x8] = 'k';
    buf[0x3] = '{';
    buf[0x18] = 'r';
    buf[0x1b] = 'h';
    buf[0xd] = '_';
    buf[0xe] = 'b';
    buf[0x7] = '1';
    buf[0x6] = 'l';
    buf[0xc] = '0';
    memset(buf, 0x41, sizeof(buf));
    printf("You like to paint? You know what I say to that? %s!", buf);
    return 0;
}