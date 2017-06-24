#ifndef _IMAGE_H
#define _IMAGE_H

#include "helper.h"

int writePpm(char *, char *, int *, int, int);
char *formattedRGB(char *dest, int *pixels, int width, int height);

#endif