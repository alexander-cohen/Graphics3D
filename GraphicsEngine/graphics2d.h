#ifndef _GRAPHICS2D_H
#define _GRAPHICS2D_H

#include <stdlib.h>
#include <stdio.h>
#include "helper.h"
#include "vector.h"

typedef struct {
	int *pixels;
	int width, height;

} g2d_buffer_info;


int g2d_set_fill (int col);
int g2d_set_stroke (int col);
int g2d_set_buff (g2d_buffer_info *buff);

int g2d_fill_bg (int col);

int g2d_draw_rect (int x, int y, int width, int height);
int g2d_fill_rect (int x, int y, int width, int height);

int g2d_draw_point (int x, int y);

int g2d_draw_line (int x1, int y1, int x2, int y2);


#endif