#ifndef HEXDUMP_H
#define HEXDUMP_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifndef _HEXDUMP_W
#define _HEXDUMP_W (32)
#endif // _HEXDUMP_W
#ifndef _HEXDUMP_S
#define _HEXDUMP_S (4)
#endif // _HEXDUMP_S
#ifndef _HEXDUMP_INDENT
#define _HEXDUMP_INDENT ""
#endif // _HEXDUMP_INDENT

void hexdump(const void *const data, const size_t sz);
void hexdump_w(const void *const data, const size_t sz, const size_t width,
               const size_t sep, const char *const indent);

#endif // HEXDUMP_H
