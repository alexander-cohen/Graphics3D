#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int *pixels;
	const int WIDTH, HEIGHT;

} buffer_info;

typedef struct {
	int x, y;
} point;

int draw_line (buffer_info &inf, const point &p1, const point &p2, int col);
