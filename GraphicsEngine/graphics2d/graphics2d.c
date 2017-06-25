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

	//avoid unnecessary function calls
	if ((graphics_context -> thickness) == 1)
	{
		g2d_set_pixel (x, y, col);
	}
	else
	{
		g2d_fill_rect (
		x - (graphics_context -> thickness)/2, y - (graphics_context -> thickness)/2, 
		(graphics_context -> thickness), (graphics_context -> thickness));
	}
	
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
	g2d_set_thickness (1);
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

static int next_line_point (
	const int x1, const int y1, 
	const int x2, const int y2, 
	int *cur_x, int *cur_y, int *cur_dist)
{	
	int overall_dx_abs = abs(x2 - x1);
	int overall_dy_abs = abs(y2 - y1);

	bool lateral = overall_dx_abs >= overall_dy_abs;

	int move_straight = *cur_dist + (lateral ? overall_dy_abs : -overall_dx_abs);
	int move_diagonally = *cur_dist + overall_dy_abs - overall_dx_abs;

	char xmov = (x2 > x1 ? 1 : -1);
	char ymov = (y2 > y1 ? 1 : -1);

	if (abs(move_straight) < abs(move_diagonally))
	{
		if (lateral)
		{
			*cur_x += xmov;
		}
		else
		{
			*cur_y += ymov;
		}
		*cur_dist = move_straight;
	}
	else
	{
		*cur_x += xmov;
		*cur_y += ymov;
		*cur_dist = move_diagonally;
	}

	return 0;
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

	do
	{
		g2d_set_thick_pixel (cur_x, cur_y, (graphics_context -> color));
		next_line_point (x1, y1, x2, y2, &cur_x, &cur_y, &cur_dist);
	} while (cur_x != x2 || cur_y != y2);

	return 0;
}


int g2d_fill_ellipse (int cx, int cy, int semimajor, int semiminor)
{
	g2d_set_thickness (1);

	int semimajor2 = semimajor * semimajor;
	int semiminor2 = semiminor * semiminor;
	int rad2 = semimajor2 * semiminor2;
	int cur_dist = rad2;

	//start at right most point
	int cur_x = cx + semimajor;
	int cur_y = cy;

	//iterate through the first quadrant simultaneously drawing all the other quadrants

	bool verbose = false;

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

int g2d_fill_triangle (
	const int x1, const int y1, 
	const int x2, const int y2,
	const int x3, const int y3)
{
	g2d_set_thickness (1);

	//make sure its not degenerate
	assert (
		!(x1 == x2 && y1 == y2) && 
		!(x1 == x3 && y1 == y3) &&
		!(x2 == x3 && y2 == y3) );

	//we split the points up into top, left, and right

	//set it randomly initially and then fix it up
	int 
	topx = x1, topy = y1, 
	leftx = x2, lefty = y2, 
	rightx = x3, righty = y3;

	//set topx and topy correctly
	if (y1 <= y2 && y1 <= y3)
	{
		topx = x1;
		topy = y1;

		leftx = x2;
		lefty = y2;

		rightx = x3;
		righty = y3;
	}
	else if (y2 <= y1 && y2 <= y3)
	{
		topx = x2;
		topy = y2;

		leftx = x1;
		lefty = y1;

		rightx = x3;
		righty = y3;
	}
	else
	{
		topx = x3;
		topy = y3;

		leftx = x1;
		lefty = y1;

		rightx = x2;
		righty = y2;
	}

	//set left and right correctly
	if (leftx > rightx)
	{
		int 
		leftx_temp = leftx,
		lefty_temp = lefty,
		rightx_temp = rightx,
		righty_temp = righty;

		leftx = rightx_temp;
		lefty = righty_temp;

		rightx = leftx_temp;
		righty = lefty_temp;
	}


	int cur_left = topx, cur_right = topx, cur_y = topy;
	int cur_y_left = cur_y, cur_y_right = cur_y;
	int left_err = 0, right_err = 0;

	if (lefty == topy || righty == topy)
	{
		int eqx = lefty == topy ? leftx : rightx;
		cur_y = topy;
		topy = -1000;
		cur_left = min (topx, eqx);
		cur_right = max (topx, eqx);

	}

	//printf ("triangle points (top, left, right): %d, %d; %d, %d; %d, %d\n", topx, topy, leftx, lefty, rightx, righty);
	do
	{
		int prev_left = cur_left, prev_right = cur_right;

		//printf ("cur y, leftx, rightx: %d, %d, %d\n", cur_y, cur_left, cur_right);
		while (cur_y_left == cur_y)
		{
			prev_left = cur_left;

			if (cur_y == lefty && cur_left == leftx)
			{
				cur_y_left++;
				break;
			}


			//printf ("%d, %d, %d\n", cur_left, cur_y_left, left_err);
			if (cur_y_left <= lefty)
				next_line_point (topx, topy, leftx, lefty, &cur_left, &cur_y_left, &left_err);
			else
				next_line_point (leftx, lefty, rightx, righty, &cur_left, &cur_y_left, &left_err);

		}

		while (cur_y_right == cur_y)
		{
			prev_right = cur_right;

			if (cur_y == righty && cur_right == rightx)
			{
				cur_y_right++;
				break;
			}

			if (cur_y_right <= righty) 
				next_line_point (topx, topy, rightx, righty, &cur_right, &cur_y_right, &right_err);
			else
				next_line_point (rightx, righty, leftx, lefty, &cur_right, &cur_y_right, &right_err);
		}

		cur_y++;
		g2d_draw_line (prev_left, cur_y, prev_right, cur_y);

	} while (cur_left <= cur_right && cur_y < max (lefty, righty));

	return 0;
}