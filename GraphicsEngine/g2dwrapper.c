#include "g2dwrapper.h"


g2d_primitive *create_line (int x1, int y1, int x2, int y2, int thickness, int col)
{
	g2d_primitive *line = (g2d_primitive *)malloc (sizeof (g2d_primitive));
	encode_line (line, x1, y1, x2, y2, thickness, col);
}
int encode_line (g2d_primitive *dst, int x1, int y1, int x2, int y2, int thickness, int col)
{
	dst -> type = LINE;
	memset (dst -> data, 0, sizeof (dst -> data));

	(dst -> data)[0] = x1;
	(dst -> data)[1] = y1;
	(dst -> data)[2] = x2;
	(dst -> data)[3] = y2;
	(dst -> data)[4] = thickness;
	(dst -> data)[5] = col;
}
int draw_encoded_line (g2d_context *graphics_context, int *data)
{
	g2d_set_context (graphics_context);
	g2d_set_col (data[5]);
	g2d_draw_thick_line (data[0], data[1], data[2], data[3], data[4]);
}

g2d_primitive *create_rect_outline (int x, int y, int width, int height, int thickness, int col);
int encode_rect_outline (g2d_primitive *dst, int x, int y, int width, int height, int thickness, int col);
int draw_encoded_rect_outline (g2d_context *graphics_context, int *data);

g2d_primitive *create_rect_fill (g2d_primitive *dst, int x, int y, int width, int height, int col);
int encode_rect_fill (g2d_primitive *dst, int x, int y, int width, int height, int col);
int draw_encoded_rect_fill (g2d_context *graphics_context, int *data);

g2d_primitive *create_circle_outline (g2d_primitive *dst, int x, int y, int rad, int thickness, int col);
int encode_circle_outline (g2d_primitive *dst, int x, int y, int rad, int thickness, int col);
int draw_encoded_circle_outline (g2d_context *graphics_context, int *data);

g2d_primitive *create_circle_fill (g2d_primitive *dst, int x, int y, int rad, int col);
int encode_circle_fill (g2d_primitive *dst, int x, int y, int rad, int col);
int draw_encoded_circle_fill (g2d_context *graphics_context, int *data);