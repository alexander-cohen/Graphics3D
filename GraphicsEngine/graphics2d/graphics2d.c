#include "graphics2d.h"


int cur_color = BLACK;
g2d_context *graphics_context;

static int *g2d_buffer_get (int r, int c) {
	assert (r >= 0 && c >= 0 && r < (graphics_context -> height) && c < (graphics_context -> width));
	return graphics_context -> pixels + r * (graphics_context -> width) + c;
}

static int g2d_set_pixel (int r, int c, int col) {
	if (r < 0 || c < 0 || r > (graphics_context -> height) || r > (graphics_context -> width))
	{
		return 1;
	}
	else
	{
		*g2d_buffer_get (r, c) = col;
		return 0;
	}
}

int g2d_set_col (int col)
{
	cur_color = col;
	return 0;
}

int g2d_set_context (g2d_context *buff)
{
	graphics_context = buff;
	return 0;
}

int g2d_fill_bg (int col)
{
	g2d_set_col (col);
	g2d_fill_rect (0, 0, graphics_context -> width, graphics_context -> height);
	return 0;
}

void checkrect (int x, int y, int width, int height)
{
	assert (x >= 0 && y >= 0);
	assert (width > 0 && height > 0);
	assert (x + width <= (graphics_context -> width) && y + height <= (graphics_context -> height));
}

void check_drawable_rect (int x, int y, int width, int height)
{
	assert (width > 0 && height > 0);
}

int g2d_draw_rect (int x, int y, int width, int height)
{
	g2d_draw_thick_rect (x, y, width, height, 1);
	return 0;
}

int g2d_draw_thick_rect (int x, int y, int width, int height, int thickness)
{
	check_drawable_rect (x, y, width, height);

	//top and bottom
	for (int c = x; c < x + width; c++)
	{
		for (int i = 0; i < thickness; i++)
		{
			g2d_set_pixel (y + i, c, cur_color);
			g2d_set_pixel (y + height - i, c, cur_color);
		}
	}

	for (int r = y; r < y + height; r++)
	{
		for (int i = 0; i < thickness; i++)
		{
			g2d_set_pixel (r, x + i, cur_color);
			g2d_set_pixel (r, x + width - i, cur_color);
		}
	}
	return 0;
}

int g2d_fill_rect (int x, int y, int width, int height)
{	
	check_drawable_rect (x, y, width, height);

	for (int r = y; r < y + height; r++)
	{
		for (int c = x; c < x + width; c++)
		{
			g2d_set_pixel(r, c, cur_color);
		}
	}

	return 0;
}

int g2d_draw_point (int x, int y)
{
	return g2d_set_pixel (y, x, cur_color);
}

int g2d_draw_line (int x1, int y1, int x2, int y2)
{
	return g2d_draw_thick_line (x1, y2, x2, y2, 1);
}

int g2d_draw_thick_line (int x1, int y1, int x2, int y2, int thickness)
{

	if (x1 == x2 && y1 == y2)
	{
		return g2d_draw_point (x1, y2);
	}

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
		int rbeg = cur_y - thickness / 2;
		int cbeg = cur_x - thickness / 2;
		g2d_fill_rect (rbeg, cbeg, thickness, thickness);

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


int g2d_fill_circle (int x, int y, int rad)
{
	int rad2 = rad * rad;
	int cur_dist = rad2;

	//start at right most point
	int cur_x = x + rad;
	int cur_y = y;

	//iterate through the first quadrant simultaneously drawing all the other quadrants

	do {

		int dx = cur_x - x;
		int dy = cur_y - y;


		g2d_draw_line (x + dx, y + dy, x - dx, y + dy);
		g2d_draw_line (x + dx, y - dy, x - dx, y - dy);
		
		int up = cur_dist + 2 * dy + 1;
		int left = cur_dist - 2 * dx + 1;
		int diag = cur_dist + 2 * dy - 2 * dx + 2;

		int up_err = abs (up - rad2);
		int left_err = abs (left - rad2);
		int diag_err = abs (diag - rad2);

		//printf ("current point: %d, %d; %d, %d; %d, %d, %d; %d\n", cur_x, cur_y, dx, dy, up, left, diag, cur_dist);

		//diagonal is optimal
		if (diag_err <= up_err && diag_err <= left_err)
		{
			cur_dist = diag;
			cur_x--;
			cur_y++;
		}

		else if (up_err <= left_err)
		{
			cur_dist = up;
			cur_y++;
		}

		else
		{
			cur_dist = left;
			cur_x--;
		}

	} while (cur_x > x);

	return 0;
}

int g2d_draw_circle (int x, int y, int rad)
{
	g2d_draw_thick_circle (x, y, rad, 1);
	return 0;
}

int g2d_draw_thick_circle (int x, int y, int rad, int thickness)
{
	int rad2 = rad * rad;
	int cur_dist = rad2;

	//start at right most point
	int cur_x = x + rad;
	int cur_y = y;

	//iterate through the first quadrant simultaneously drawing all the other quadrants

	do {

		int dx = cur_x - x;
		int dy = cur_y - y;

		//current point, in quad 1
		int rbeg = x + dx - thickness / 2;
		int cbeg = y + dy - thickness / 2;
		g2d_fill_rect (rbeg, cbeg, thickness, thickness);

		//current point in quad2, reflected over y-axis
		rbeg = x - dx - thickness / 2;
		cbeg = y + dy - thickness / 2;
		g2d_fill_rect (rbeg, cbeg, thickness, thickness);

		//current point in quad3, reflected over origin
		rbeg = x - dx - thickness / 2;
		cbeg = y - dy - thickness / 2;
		g2d_fill_rect (rbeg, cbeg, thickness, thickness);

		//current point in quad4, reflected over x-axis
		rbeg = x + dx - thickness / 2;
		cbeg = y - dy - thickness / 2;
		g2d_fill_rect (rbeg, cbeg, thickness, thickness);

		

		int up = cur_dist + 2 * dy + 1;
		int left = cur_dist - 2 * dx + 1;
		int diag = cur_dist + 2 * dy - 2 * dx + 2;

		int up_err = abs (up - rad2);
		int left_err = abs (left - rad2);
		int diag_err = abs (diag - rad2);

		//printf ("current point: %d, %d; %d, %d; %d, %d, %d; %d\n", cur_x, cur_y, dx, dy, up, left, diag, cur_dist);

		//diagonal is optimal
		if (diag_err <= up_err && diag_err <= left_err)
		{
			cur_dist = diag;
			cur_x--;
			cur_y++;
		}

		else if (up_err <= left_err)
		{
			cur_dist = up;
			cur_y++;
		}

		else
		{
			cur_dist = left;
			cur_x--;
		}

	} while (cur_x > x);

	return 0;
}

int g2d_fill_triangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
	return 0;
}