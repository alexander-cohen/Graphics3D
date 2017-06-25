#include "graphics2d.h"

g2d_context *graphics_context;

g2d_context *g2d_create_graphics_context (int *pixels, int width, int height)
{
	g2d_context *graphics_context = (g2d_context *) malloc (sizeof (g2d_context));
    graphics_context -> pixels = pixels;
    graphics_context -> width = width;
    graphics_context -> height = height;

    graphics_context -> color = BLACK;
    graphics_context -> thickness = 1;

    return graphics_context;
}




static inline int *g2d_buffer_get (int r, int c) {
	assert (r >= 0 && c >= 0 && r < (graphics_context -> height) && c < (graphics_context -> width));
	return graphics_context -> pixels + r * (graphics_context -> width) + c;
}

static inline int g2d_set_pixel (int x, int y, int col) {
	if (x < 0 || y < 0 || x > (graphics_context -> width) || y > (graphics_context -> height))
	{
		return 1;
	}
	else
	{
		*g2d_buffer_get (y, x) = col;
		return 0;
	}
}

static int g2d_set_thick_pixel (int x, int y, int col) {
	g2d_set_col (col);
	g2d_fill_rect (
		x - (graphics_context -> thickness)/2, y - (graphics_context -> thickness)/2, 
		(graphics_context -> thickness), (graphics_context -> thickness));

	return 0;
}

int g2d_set_col (int col)
{
	(graphics_context -> color) = col;
	return 0;
}

int g2d_set_thickness (int thickness)
{
	(graphics_context -> thickness) = thickness;
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

int g2d_draw_rect (int x, int y, int width, int height)
{
	assert (width > 0 && height > 0);

	//top and bottom
	for (int px = x; px < x + width; px++)
	{
		g2d_set_thick_pixel (px, y, (graphics_context -> color));
		g2d_set_thick_pixel (px, y + height, (graphics_context -> color));
	}

	for (int py = y; py < y + height; py++)
	{
		g2d_set_thick_pixel (x, py, (graphics_context -> color));
		g2d_set_thick_pixel (x + width, py, (graphics_context -> color));
	}

	return 0;
}

int g2d_fill_rect (int x, int y, int width, int height)
{	
	assert (width > 0 && height > 0);

	//iterate by y first bc 2d arrays are faster that way
	for (int py = y; py < y + height; py++)
	{
		for (int px = x; px < x + width; px++)
		{
			g2d_set_pixel (px, py, (graphics_context -> color));
		}
	}

	return 0;
}

int g2d_draw_point (int x, int y)
{
	return g2d_set_thick_pixel (x, y, (graphics_context -> color));
}

int g2d_draw_line (int x1, int y1, int x2, int y2)
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
		g2d_set_thick_pixel (cur_x, cur_y, (graphics_context -> color));
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


int g2d_fill_ellipse (int cx, int cy, int semimajor, int semiminor)
{
	int semimajor2 = semimajor * semimajor;
	int semiminor2 = semiminor * semiminor;
	int rad2 = semimajor2 * semiminor2;
	int cur_dist = rad2;

	//start at right most point
	int cur_x = cx + semimajor;
	int cur_y = cy;

	//iterate through the first quadrant simultaneously drawing all the other quadrants

	bool verbose = true;

	do {
		int dx = cur_x - cx;
		int dy = cur_y - cy;

		g2d_draw_line (cx + dx, cy + dy, cx - dx, cy + dy);
		g2d_draw_line (cx + dx, cy - dy, cx - dx, cy - dy);

		int up = cur_dist + semimajor2 * (2 * dy + 1);
		int left = cur_dist + semiminor2 * (- 2 * dx + 1);
		int diag = up + left - cur_dist;

		int up_err = abs (up - rad2);
		int left_err = abs (left - rad2);
		int diag_err = abs (diag - rad2);

		if (verbose) 
			printf ("current point: %d, %d; %d, %d; %d, %d, %d; %d", cur_x, cur_y, dx, dy, up, left, diag, cur_dist);

		//diagonal is optimal
		if (diag_err <= up_err && diag_err <= left_err)
		{
			cur_dist = diag;
			cur_x--;
			cur_y++;

			if (verbose)
				printf("; DIAG\n");
		}

		else if (up_err <= left_err)
		{
			cur_dist = up;
			cur_y++;

			if (verbose)
				printf("; UP\n");
		}

		else
		{
			cur_dist = left;
			cur_x--;

			if (verbose)
				printf("; LEFT\n");
		}

	} while (cur_x > cx);

	return 0;
}

int g2d_draw_ellipse (int cx, int cy, int semimajor, int semiminor)
{
	int semimajor2 = semimajor * semimajor;
	int semiminor2 = semiminor * semiminor;
	int rad2 = semimajor2 * semiminor2;
	int cur_dist = rad2;

	//start at right most point
	int cur_x = cx + semimajor;
	int cur_y = cy;

	//iterate through the first quadrant simultaneously drawing all the other quadrants

	bool verbose = true;

	do {
		int dx = cur_x - cx;
		int dy = cur_y - cy;

		g2d_set_thick_pixel (cx + dx, cy + dy, (graphics_context -> color));
		g2d_set_thick_pixel (cx - dx, cy + dy, (graphics_context -> color));
		g2d_set_thick_pixel (cx + dx, cy - dy, (graphics_context -> color));
		g2d_set_thick_pixel (cx - dx, cy - dy, (graphics_context -> color));

		int up = cur_dist + semimajor2 * (2 * dy + 1);
		int left = cur_dist + semiminor2 * (- 2 * dx + 1);
		int diag = up + left - cur_dist;

		int up_err = abs (up - rad2);
		int left_err = abs (left - rad2);
		int diag_err = abs (diag - rad2);

		if (verbose) 
			printf ("current point: %d, %d; %d, %d; %d, %d, %d; %d", cur_x, cur_y, dx, dy, up, left, diag, cur_dist);

		//diagonal is optimal
		if (diag_err <= up_err && diag_err <= left_err)
		{
			cur_dist = diag;
			cur_x--;
			cur_y++;

			if (verbose)
				printf("; DIAG\n");
		}

		else if (up_err <= left_err)
		{
			cur_dist = up;
			cur_y++;

			if (verbose)
				printf("; UP\n");
		}

		else
		{
			cur_dist = left;
			cur_x--;

			if (verbose)
				printf("; LEFT\n");
		}

	} while (cur_x > cx);

	return 0;
}

int g2d_fill_triangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
	return 0;
}