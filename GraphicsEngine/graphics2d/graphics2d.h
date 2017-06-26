#ifndef GRAPHICS2D_H
#define GRAPHICS2D_H

#include <stdlib.h>
#include <stdio.h>
#include "helper.h"
#include "stack.h"
#include "displayer.h"

#define FIXED_POINT_SIZE 8

typedef struct {
	int *pixels;
	int width, height;
	int color, thickness;
	stack cs_stack;
} g2d_context;

g2d_context *g2d_create_graphics_context (int *pixels, int width, int height);

int g2d_set_context (g2d_context *buff);
int g2d_set_col (int col);
int g2d_set_thickness (int thickness);

int g2d_fill_bg (int col);

int g2d_draw_point (int x, int y);

int g2d_draw_line (int x1, int y1, int x2, int y2);

int g2d_fill_rect (int x, int y, int width, int height);
int g2d_draw_rect (int x, int y, int width, int height);

int g2d_fill_ellipse (int cx, int cy, int semimajor, int semiminor);
int g2d_draw_ellipse (int cx, int cy, int semimajor, int semiminor);

int g2d_fill_ellipse_inrect (int x, int y, int width, int height);
int g2d_draw_ellipse_inrect (int x, int y, int width, int height);


int g2d_fill_triangle (
	const int x1, const int y1, 
	const int x2, const int y2,
	const int x3, const int y3);

int g2d_fill_triangle_boundingbox (
	const short x1, const short y1, 
	const short x2, const short y2,
	const short x3, const short y3);

int g2d_fill_triangle_boundingbox_baseline (
	const short x1, const short y1, 
	const short x2, const short y2,
	const short x3, const short y3);

int g2d_fill_triangle_boundingbox_avx (
	const int x1, const int y1, 
	const int x2, const int y2,
	const int x3, const int y3);
#endif