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


int draw_line_octant1 (buffer_info *inf, const point p1, const point p2, int col, bool verbose)
{
	int dx = p2.x - p1.x;
	int dy = p2.y = p1.y;

	//ensures we are in octant 1
	if (dx < 0 || dy < 0 || dy < dx)
	{
		return 1;
	}

	int curx = p1.x;
	int cury = p1.y;

	do
	{
		*get_buf (inf, cur_y, cur_x) = col;

		

	} while (cur_x != p2.x && cur_y != p2.y)

}

int draw_line (buffer_info *inf, const point p1, const point p2, int col, bool verbose)
{
	if (p1.x < 0 || p1.x >= (inf -> width) ||
		p1.y < 0 || p1.y >= (inf -> height) ||
		p2.x < 0 || p2.x >= (inf -> width)|| 
		p2.y < 0 || p2.y >= (inf -> height))
	{
		return 1;
	}
	
	if (verbose)
	{
		printf ("line points: %d, %d, %d, %d\n", p1.x, p1.y, p2.x, p2.y);
	}

	int cur_x = p1.x;
	int cur_y = p1.y;

	int xdir = p2.x > cur_x ? 1 : -1;
	int ydir = p2.y > cur_y ? 1 : -1;

	do
	{
		*get_buf (inf, cur_y, cur_x) = col;

		//delta x is either -1, 0, 1 and delta y is either -1, 0, 1
		//determine each independentally
		bool dec_dx = false, dec_dy = false;

		int dy = p2.y - cur_y;
		int dx = p2.x - cur_x;

		int desired_dy = p2.y - p1.y;
		int desired_dx = p2.x - p1.x;

		if (verbose)
		{
			printf ("cur pos: %d, %d, compare slope: %d\n", 
				cur_x, cur_y, compare_slope (dx, dy, desired_dx, desired_dy));
		}
		
		//determine if we need to move vertically or horizontally by comparing slopes
		switch (compare_slope (dx, dy, desired_dx, desired_dy))
		{
			//our slope is less than the desired slope
			case -1:
				dec_dx = true; //decrease the x differences to increase the slope
				
				//if our slope is now too high, move diagonally
				if (compare_slope (dx -1, dy, desired_dx, desired_dy) == 1)
				{
					dec_dy = true;
				}
				break;

			//our slope is greater than the desired slope
			case 1:
				dec_dy = true; //decrease the y difference to decrease the slope

				//if our slope is now too low, move diagonally
				if (compare_slope (dx, dy - 1, desired_dx, desired_dy) == -1)
				{
					dec_dx = true;
				}
				break;

			//our slope is equal to the desired slope, so move diagonally
			case 0:
				dec_dx = true;
				dec_dy = true;
				break;
		}

		cur_x += dec_dx ? xdir : 0;
		cur_y += dec_dy ? ydir : 0;

	} while (cur_x != p2.x && cur_y != p2.y);
}


int draw_circle (buffer_info *inf, const point p, int rad, int col, bool verbose)
{

}