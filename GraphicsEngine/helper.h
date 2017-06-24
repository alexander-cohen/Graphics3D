#ifndef _HELPER_H
#define _HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "colors.h"

#define rgbToHex(r, g, b) ((r << 16) + (g << 8) + b)

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define bool char
#define true 1
#define false 0

#endif