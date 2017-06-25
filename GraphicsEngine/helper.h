#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "colors.h"

#define rgbToHex(r, g, b) ((r << 16) + (g << 8) + b)

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define max3(a, b, c) max(max(a, b), c)
#define min3(a, b, c) min(min(a, b), c)

#define bool char
#define true 1
#define false 0

inline void swap (int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

#endif