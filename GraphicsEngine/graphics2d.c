#include "graphics2d.h"


int cur_fill = BLACK;
int cur_stroke = WHITE;
g2d_buffer_info *g2d_buff;

static int *g2d_buffer_get (int r, int c) {
	return g2d_buff -> pixels + r * (g2d_buff -> width) + c;
}

int g2d_set_fill (int col)
{
	cur_fill = col;
}

int g2d_set_stroke (int col)
{
	cur_stroke = col;
}

int g2d_set_buff (g2d_buffer_info *buff)
{
	g2d_buff = buff;
}


int g2d_fill_bg (int col)
{
	g2d_set_fill (col);
	g2d_fill_rect (0, 0, g2d_buff -> width, g2d_buff -> height);
}

int g2d_draw_rect (int x, int y, int width, int height)
{

}

int g2d_fill_rect (int x, int y, int width, int height)
{	
	assert (x >= 0 && y >= 0);
	assert (width > 0 && height > 0);
	assert (x + width <= (g2d_buff -> width) && y + height <= (g2d_buff -> height));

	for (int r = y; r < y + height; r++)
	{
		for (int c = x; c < x + width; c++)
		{
			*g2d_buffer_get (r, c) = cur_fill;
		}
	}
}

int g2d_draw_point (int x, int y)
{

}


int g2d_draw_line (int x1, int y1, int x2, int y2)
{
	int overall_dx_abs = abs(x2 - x1);
	int overall_dy_abs = abs(y2 - y1);

	int cur_x = x1;
	int cur_y = y1;
	int cur_dist = 0;

	int xmov = x2 > x1 ? 1 : -1;
	int ymov = y2 > y1 ? 1 : -1;

	bool lateral = overall_dx_abs >= overall_dy_abs;

	do
	{
		*g2d_buffer_get (cur_y, cur_x) = cur_stroke;		

		//minimize distance from line
		//minimize |-overall_dy_abs * cur_dx + overall_dx_abs * cur_dy|

		int move_straight = cur_dist + (lateral ? overall_dy_abs : -overall_dx_abs);
		int move_diagonally = cur_dist + overall_dy_abs - overall_dx_abs;

		if (abs(move_straight) < abs(move_diagonally))
		{
			if (lateral)
			{
				cur_x += xmov;
			}
			else
			{
				cur_y += ymov;
			}
			cur_dist = move_straight;
		}
		else
		{
			cur_x += xmov;
			cur_y += ymov;
			cur_dist = move_diagonally;
		}

	} while (cur_x != x2 || cur_y != y2);

	return 0;
}
