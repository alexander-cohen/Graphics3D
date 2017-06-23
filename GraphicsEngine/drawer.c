#include "drawer.h"

int draw_line (buffer_info &inf, const point &p1, const point &p2, int col)
{
	if (p1.x < 0 || p1.x >= inf.WIDTH ||
		p1.y < 0 || p1.y >= inf.HEIGHT ||
		p2.x < 0 || p2.x >= inf.HEIGHT || 
		p2.y < 0 || p2.y >= inf.HEIGHT)
	{
		return -1;
	}

	//order the points correctly, guarantee p1 is to the left of p2
	if (p1.x > p2.x)
	{
		return draw_line (inf, p2, p1, col);
	}

	//degenerate
	else if (p1.x == p2.x)
	{
		int lower_y = min (p1.y, p2.y);
		int upper_y = max (p1.y, p2.y);	

		for (int y = lower_y; y <= upper_y; y++)
		{
			int r = y;
			int c = p1.x;

			inf.pixels[r][c] = col;
		}
		return 0;
	}

	else
	{
		int ydif = (p2.y - p1.y);
		int xdif = (p2.x - p1.x);

		//iterate through x values between the points, then draw all the y values
		for (int x = p1.x, x <= p2.x; x++)
		{
			int y1 = (ydif * (p1.x - x)) / xdif + p1.y;
			int y2 = (ydif * (p1.x - x + 1)) / xdif + p1.y;
			int miny = min (y1, y2);
			int maxy = max (y1, y2);

			for (int y = miny; y <= maxy; y++)
			{
				int r = y;
				int c = x;

				inf.pixels[r][c] = col;
			}
		}

		return 0;
	}
	
}