#ifndef _G2D_WRAPPER_H
#define _G2D_WRAPPER_H

#include "helper.h"
#include "graphics2d.h"

typedef struct
{
	int type;
	int data[15];
} g2d_primitive;

enum primtive_types {LINE, RECT_FILL, RECT_OUTLINE, CIRCLE_FILL, CIRCLE_OUTLINE};

g2d_primitive *create_line (int x1, int y1, int x2, int y2, int thickness, int col);
int encode_line (g2d_primitive *dst, int x1, int y1, int x2, int y2, int thickness, int col);
int draw_encoded_line (g2d_context *graphics_context, int *data);

g2d_primitive *create_rect_outline (int x, int y, int width, int height, int thickness, int col);
int encode_rect_outline (g2d_primitive *dst, int x, int y, int width, int height, int thickness, int col);
int draw_encoded_rect_outline (g2d_context *graphics_context, int *data);

g2d_primitive *create_rect_fill (int x, int y, int width, int height, int col);
int encode_rect_fill (g2d_primitive *dst, int x, int y, int width, int height, int col);
int draw_encoded_rect_fill (g2d_context *graphics_context, int *data);

g2d_primitive *create_circle_outline (int x, int y, int rad, int thickness, int col);
int encode_circle_outline (g2d_primitive *dst, int x, int y, int rad, int thickness, int col);
int draw_encoded_circle_outline (g2d_context *graphics_context, int *data);

g2d_primitive *create_circle_fill (int x, int y, int rad, int col);
int encode_circle_fill (g2d_primitive *dst, int x, int y, int rad, int col);
int draw_encoded_circle_fill (g2d_context *graphics_context, int *data);

int draw_primitive (g2d_context *graphics_context, g2d_primitive *primitive);

#endif