#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "colors.h"
#include <unistd.h>
#include <x86intrin.h>
//#include <immintrin.h>

#define rgbToHex(r, g, b) ((r << 16) + (g << 8) + b)

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define max3(a, b, c) max(max(a, b), c)
#define min3(a, b, c) min(min(a, b), c)

#define bool char
#define true 1
#define false 0

#define NANO_PER_SECOND 1000000000L

typedef unsigned long long ui64;
typedef long long i64;

int swap_int (int *a, int *b);


#endif