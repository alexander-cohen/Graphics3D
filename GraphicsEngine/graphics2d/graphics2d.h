#ifndef _GRAPHICS2D_H
#define _GRAPHICS2D_H

#include <stdlib.h>
#include <stdio.h>
#include "helper.h"
#include "vector.h"

typedef struct {
	int *pixels;
	int width, height;

} g2d_context;


int g2d_set_col (int col);
int g2d_set_context (g2d_context *buff);

int g2d_fill_bg (int col);

int g2d_draw_rect (int x, int y, int width, int height);
int g2d_draw_thick_rect (int x, int y, int width, int height, int thickness);

int g2d_fill_rect (int x, int y, int width, int height);

int g2d_draw_point (int x, int y);

int g2d_draw_line (int x1, int y1, int x2, int y2);
int g2d_draw_thick_line (int x1, int y1, int x2, int y2, int thickness);

int g2d_draw_circle (int x, int y, int rad);


#endif