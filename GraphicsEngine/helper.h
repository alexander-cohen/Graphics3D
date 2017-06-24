#ifndef _HELPER_H
#define _HELPER_H

#include "stdlib.h"
#include "stdio.h"

#define rgbToHex(r, g, b) ((r << 16) + (g << 8) + b)

#define BLACK   0x000000
#define WHITE   0xFFFFFF
#define RED     0xFF0000
#define LIME    0x00FF00
#define BLUE    0x0000FF
#define YELLOW  0xFFFF00
#define CYAN    0x00FFFF
#define MAGENTA 0xFF00FF
#define SILVER  0xC0C0C0
#define GRAY    0x808080
#define MAROON  0x800000
#define OLIVE   0x808000
#define GREEN   0x008000
#define PURPLE  0x800080
#define TEAL    0x008080
#define NAVY    0x000080

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define bool char
#define true 1
#define false 0

#endif