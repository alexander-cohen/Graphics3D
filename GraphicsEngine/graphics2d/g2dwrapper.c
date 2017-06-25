#include "g2dwrapper.h"


g2d_primitive *create_line (int x1, int y1, int x2, int y2, int thickness, int col)
{
	g2d_primitive *line = (g2d_primitive *)malloc (sizeof (g2d_primitive));
	encode_line (line, x1, y1, x2, y2, thickness, col);
	return line;
}
int encode_line (g2d_primitive *dst, int x1, int y1, int x2, int y2, int thickness, int col)
{
	(dst -> type) = LINE;
	memset (dst -> data, 0, sizeof (dst -> data));

	(dst -> data)[0] = x1;
	(dst -> data)[1] = y1;
	(dst -> data)[2] = x2;
	(dst -> data)[3] = y2;
	(dst -> data)[4] = thickness;
	(dst -> data)[5] = col;

	return 0;
}
int draw_encoded_line (g2d_context *graphics_context, int *data)
{
	g2d_set_context (graphics_context);
	g2d_set_col (data[5]);
	g2d_draw_thick_line (data[0], data[1], data[2], data[3], data[4]);

	return 0;
}

g2d_primitive *create_rect_outline (int x, int y, int width, int height, int thickness, int col)
{
	g2d_primitive *rect_outline = (g2d_primitive *)malloc (sizeof (g2d_primitive));
	encode_rect_outline (rect_outline, x, y, width, height, thickness, col);
	return rect_outline;
	
}
int encode_rect_outline (g2d_primitive *dst, int x, int y, int width, int height, int thickness, int col)
{
	(dst -> type) = RECT_OUTLINE;
	memset (dst -> data, 0, sizeof (dst -> data));

	(dst -> data)[0] = x;
	(dst -> data)[1] = y;
	(dst -> data)[2] = width;
	(dst -> data)[3] = height;
	(dst -> data)[4] = thickness;
	(dst -> data)[5] = col;

	return 0;
}
int draw_encoded_rect_outline (g2d_context *graphics_context, int *data)
{
	g2d_set_context (graphics_context);
	g2d_set_col (data[5]);
	g2d_draw_thick_rect (data[0], data[1], data[2], data[3], data[4]);
}

g2d_primitive *create_rect_fill (int x, int y, int width, int height, int col)
{
	g2d_primitive *rect_fill = (g2d_primitive *)malloc (sizeof (g2d_primitive));
	encode_rect_fill (rect_fill, x, y, width, height, thickness, col);
	return rect_fill;
	
}
int encode_rect_fill (g2d_primitive *dst, int x, int y, int width, int height, int col)
{
	(dst -> type) = RECT_FILL;
	memset (dst -> data, 0, sizeof (dst -> data));

	(dst -> data)[0] = x;
	(dst -> data)[1] = y;
	(dst -> data)[2] = width;
	(dst -> data)[3] = height;
	(dst -> data)[4] = col;

	return 0;
}
int draw_encoded_rect_fill (g2d_context *graphics_context, int *data)
{
	g2d_set_context (graphics_context);
	g2d_set_col (data[4]);
	g2d_fill_rect (data[0], data[1], data[2], data[3]);
}

g2d_primitive *create_circle_outline (int x, int y, int rad, int thickness, int col)
{
	g2d_primitive *circle_outline = (g2d_primitive *)malloc (sizeof (g2d_primitive));
	encode_circle_outline (circle_outline, x, y, width, height, thickness, col);
	return circle_outline;
	
}
int encode_circle_outline (g2d_primitive *dst, int x, int y, int rad, int thickness, int col)
{
	(dst -> type) = CIRCLE_OUTLINE;
	memset (dst -> data, 0, sizeof (dst -> data));

	(dst -> data)[0] = x;
	(dst -> data)[1] = y;
	(dst -> data)[2] = rad;
	(dst -> data)[3] = thickness;
	(dst -> data)[4] = col;

	return 0;
}
int draw_encoded_circle_outline (g2d_context *graphics_context, int *data)
{
	g2d_set_context (graphics_context);
	g2d_set_col (data[4]);
	g2d_draw_circle_outline (data[0], data[1], data[2], data[3]);
}

g2d_primitive *create_circle_fill (int x, int y, int rad, int thickness, int col)
{
	g2d_primitive *circle_fill = (g2d_primitive *)malloc (sizeof (g2d_primitive));
	g2d_draw_thick_circle (circle_fill, x, y, width, height, thickness, col);
	return circle_fill;
	
}
int encode_circle_fill (g2d_primitive *dst, int x, int y, int rad, int thickness, int col)
{
	(dst -> type) = CIRCLE_FILL;
	memset (dst -> data, 0, sizeof (dst -> data));

	(dst -> data)[0] = x;
	(dst -> data)[1] = y;
	(dst -> data)[2] = rad;
	(dst -> data)[3] = col;

	return 0;
}
int draw_encoded_circle_fill (g2d_context *graphics_context, int *data)
{
	g2d_set_context (graphics_context);
	g2d_set_col (data[3]);
	g2d_fill_circle (data[0], data[1], data[2]);
}

int draw_primitive (g2d_context *graphics_context, g2d_primitive *primitive)
{
	switch (primitive -> type)
	{
		case LINE:
			draw_encoded_line (graphics_context, primitive -> data);
			break;
		case RECT_OUTLINE:
			draw_encoded_rect_outline (graphics_context, primitive -> data);
			break;
		case RECT_FILL:
			draw_encoded_rect_fill (graphics_context, primitive -> data);
			break;
		case CIRCLE_OUTLINE:
			draw_encoded_circle_outline (graphics_context, primitive -> data);
			break;
		case CIRCLE_FILL:
			draw_encoded_circel_fill (graphics_context, primitive -> data);
			break;
		default:
			printf ("did not find matching primitive.\n");
			return 1;
	}

	return 0;
}