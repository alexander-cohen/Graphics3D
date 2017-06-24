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

int *get_buf (buffer_info *inf, int r, int c);

int draw_line (buffer_info *inf, const Vec2 p1, const Vec2 p2, int col, bool verbose);
int draw_circle (buffer_info *inf, const Vec2 p, int rad, int col, bool verbose);

#endif