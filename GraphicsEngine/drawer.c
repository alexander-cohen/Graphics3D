#include "drawer.h"


int *get_buf (buffer_info *inf, int r, int c) {
	return inf -> pixels + r * (inf -> width) + c;
}

//helper function, compares two slopes
//returns -1 if first slope is less than second slope, 1 if greater, 0 if equal
int compare_slope (int dx, int dy, int desired_dx, int desired_dy)
{
	int left = dy * desired_dx, right = desired_dy * dx;
	if (left < right)
	{
		return -1;
	}

	else if (left > right)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}


int draw_line (buffer_info *inf, const point p1, const point p2, int col)
{
	int overall_dx_abs = abs(p2.x - p1.x);
	int overall_dy_abs = abs(p2.y - p1.y);

	int cur_x = p1.x;
	int cur_y = p1.y;
	int cur_dist = 0;

	int xmov = p2.x > p1.x ? 1 : -1;
	int ymov = p2.y > p1.y ? 1 : -1;

	bool lateral = overall_dx_abs >= overall_dy_abs;

	do
	{
		*get_buf (inf, cur_y, cur_x) = col;		

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

	} while (cur_x != p2.x || cur_y != p2.y);

	return 0;
}


int draw_circle (buffer_info *inf, const point p, int rad, int col)
{
	return 0;
}