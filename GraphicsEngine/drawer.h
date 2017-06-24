#ifndef _DRAWER_H
#define _DRAWER_H

#include <stdlib.h>
#include <stdio.h>
#include "helper.h"
#include "vector.h"

typedef struct {
	int *pixels;
	int width, height;

} buffer_info;

typedef struct {
	int x, y;
} point;

int *get_buf (buffer_info *inf, int r, int c);

int draw_line (buffer_info *inf, const point p1, const point p2, int col);
int draw_circle (buffer_info *inf, const point p, int rad, int col);

#endif