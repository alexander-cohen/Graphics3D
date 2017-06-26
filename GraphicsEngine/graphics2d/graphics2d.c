
#include "graphics2d.h"

g2d_context *graphics_context;

g2d_context *g2d_create_graphics_context (int *pixels, int width, int height)
{
	g2d_context *graphics_context = (g2d_context *) malloc(sizeof(g2d_context));
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
	if (x < 0 || y < 0 || x >= (graphics_context -> width) || y >= (graphics_context -> height))
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

int draw_horizontal_line (int left, int right, int y)
{
	if (left > right) 
	{
		return draw_horizontal_line (right, left, y);
	}
	for (int x = left; x <= right; x++)
	{
		g2d_set_pixel (x, y, (graphics_context -> color));
	}
}

int cross2d (int dx1, int dy1, int dx2, int dy2)
{
	return dx1 * dy2 - dx2 * dy1;
}

int orient2d (short x1, short y1, short x2, short y2, short x3, short y3)
{
	return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

int g2d_fill_triangle_boundingbox (
	const short x1, const short y1, 
	const short x2, const short y2,
	const short x3, const short y3)
{

	//y's are negated bc the axis is inverted
	short 
	dx12 = (x2 - x1), dy12 = (y2 - y1),
	dx23 = (x3 - x2), dy23 = (y3 - y2),
	dx31 = (x1 - x3), dy31 = (y1 - y3);

	//change the order so its counter clockwise
	if (cross2d (dx12, dy12, dx23, dy23) < 0)
	{
		return g2d_fill_triangle_boundingbox (x1, y1, x3, y3, x2, y2);
	}

	//printf ("points: %d, %d; %d, %d; %d %d; %d\n", x1, y1, x2, y2, x3, y3, cross2d (dx12, dy12, dx23, dy23));
	//printf ("difs: %d, %d; %d, %d; %d %d\n", dx12, dy12, dx23, dy23, dx31, dy31);


	short min_x = min3 (x1, x2, x3);
	short min_y = min3 (y1, y2, y3);

	short max_x = max3 (x1, x2, x3);
	short max_y = max3 (y1, y2, y3);

	//printsf ("min: %d, %d\n", min_x, min_y);

	int w1_row = orient2d (x2, y2, x3, y3, min_x, min_y);
	int w2_row = orient2d (x3, y3, x1, y1, min_x, min_y);
	int w3_row = orient2d (x1, y1, x2, y2, min_x, min_y);


	for (short y = min_y; y <= max_y; y++)
	{
		int w1 = w1_row;
		int w2 = w2_row;
		int w3 = w3_row;
		bool found = false;

		for (short x = min_x; x <= max_x; x++)
		{
			if ((w1 | w2 | w3) >= 0)
			{
				g2d_set_pixel (x, y, (graphics_context -> color));
				found = true;
			}

			//once there are no more pixels, skip to the next scanline
			else if (found == true)
			{
				break;
			}

			w1 -= dy23;
			w2 -= dy31;
			w3 -= dy12;
		}

		w1_row += dx23;
		w2_row += dx31;
		w3_row += dx12;
	}
}

__m256i scalar_gt(__m256i a, __m256i b) {
	__m256i out;
	int *outpt = &out;
	int *apt = &a;
	int *bpt = &b;
	int i = 0;
	for(; i < 8; i++) {
		outpt[i] = apt[i] > bpt[i];
	}
	return out;
}

__m256i scalar_3and(__m256i a, __m256i b, __m256i c) {
	__m256i out;
	int *outpt = &out;
	int *apt = &a;
	int *bpt = &b;
	int *cpt = &c;
	int i = 0;
	for(; i < 8; i++) {
		outpt[i] = apt[i] & bpt[i] & cpt[i];
	}
	return out;
}

printvec(__m256i v) {
	int *p = &v;
	printf("{%d %d %d %d %d %d %d %d}", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
}

int g2d_fill_triangle_boundingbox_avx2 (
	const short x1, const short y1, 
	const short x2, const short y2,
	const short x3, const short y3)
{

	//y's are negated bc the axis is inverted
	short 
	dx12 = (x2 - x1), dy12 = (y2 - y1),
	dx23 = (x3 - x2), dy23 = (y3 - y2),
	dx31 = (x1 - x3), dy31 = (y1 - y3);

	//change the order so its counter clockwise
	if (cross2d (dx12, dy12, dx23, dy23) < 0)
	{
		return g2d_fill_triangle_boundingbox_avx2 (x1, y1, x3, y3, x2, y2);
	}

	//printf ("points: %d, %d; %d, %d; %d %d; %d\n", x1, y1, x2, y2, x3, y3, cross2d (dx12, dy12, dx23, dy23));
	//printf ("difs: %d, %d; %d, %d; %d %d\n", dx12, dy12, dx23, dy23, dx31, dy31);


	short min_x = min3 (x1, x2, x3);
	short min_y = min3 (y1, y2, y3);

	short max_x = max3 (x1, x2, x3);
	short max_y = max3 (y1, y2, y3);

	//printsf ("min: %d, %d\n", min_x, min_y);

	int w1_row = orient2d (x2, y2, x3, y3, min_x, min_y);
	int w2_row = orient2d (x3, y3, x1, y1, min_x, min_y);
	int w3_row = orient2d (x1, y1, x2, y2, min_x, min_y);

	__m256i ran8 = _mm256_set_epi32(-8, -7, -6, -5, -4, -3, -2, -1);
	__m256i w1vinc = _mm256_set1_epi32(dy23 << 3);
	__m256i w2vinc = _mm256_set1_epi32(dy31 << 3);
	__m256i w3vinc = _mm256_set1_epi32(dy12 << 3);
	int m1inc = dy23 << 3;
	int m2inc = dy31 << 3;
	int m3inc = dy12 << 3;
	__m256i w1v_row = _mm256_mullo_epi32(_mm256_set1_epi32(dy23), ran8) + _mm256_set1_epi32(w1_row);
	__m256i w2v_row = _mm256_mullo_epi32(_mm256_set1_epi32(dy31), ran8) + _mm256_set1_epi32(w2_row);
	__m256i w3v_row = _mm256_mullo_epi32(_mm256_set1_epi32(dy12), ran8) + _mm256_set1_epi32(w3_row);
	printf("w1v_row: ");
	printvec(w1v_row);
	printf("\nw2v_row: ");
	printvec(w2v_row);
	printf("\nw3v_row: ");
	printvec(w3v_row);
	__m256i w1v;
	__m256i w2v;
	__m256i w3v;
	__m256i w1vinc_row = _mm256_set1_epi32(dx23);
	__m256i w2vinc_row = _mm256_set1_epi32(dx31);
	__m256i w3vinc_row = _mm256_set1_epi32(dx12);
	__m256i v0 = _mm256_set1_epi32(0);
	__m256i v1 = _mm256_set1_epi32(1);
	__m256i condval;
	int i = min_x;
	int iinc = graphics_context->width;
	int ireset = iinc + min_x;
	
	for (short y = min_y; y <= max_y; y++)
	{
		w1v = w1v_row;
		w2v = w2v_row;
		w3v = w3v_row;
		bool found = false;

		for (short x = min_x; x <= max_x; x+=8, i+=8)
		{
			condval = _mm256_and_si256(_mm256_cmpgt_epi32(w1v, v0), _mm256_and_si256(_mm256_cmpgt_epi32(w2v, v0), _mm256_cmpgt_epi32(w3v, v0)));
			//int *ptr = &condval;
			// for(j = 0; j < 8; j++) {
				// 	if((x + j) < 20 && y < 20) {
				// 		printf("point (%d, %d) tested, ptr[j] = %d\n", x + j, y, ptr[j]);
				// 	}
				// 	if (ptr[j] && x + j >= 0 && x + j < graphics_context->width && y >= 0 && y < graphics_context->height) {
						
				// 		(graphics_context->pixels)[i + j] = graphics_context->color;
				// 	}
				// }
			//printf("condval: %d %d %d %d %d %d %d %d\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
			// if(!_mm256_testc_si256(condval, v1)) { // condval && true == false, condval = false
			// 	if(!found) {
			// 		found = true; // condval has trues
			if(i <= 100)
			printf("%d\n", i);
			      	memcpy((graphics_context->pixels) + i, &condval, 256);
								
			// 	}
			// }
			// 	// else { // found is true, check if there are any false vals
			// 	// 	if(_mm)
			// 	// }
			// //}
			// else if(found) { // none are true, but previously were
			// 	break;
			// }
			
			w1v -= w1vinc;
			w2v -= w2vinc;
			w3v -= w3vinc;
		}
		i = ireset;
		ireset += iinc;
		w1v_row += w1vinc_row;
		w2v_row += w2vinc_row;
		w3v_row += w3vinc_row;
	}
}



void print_vec (__m256i vec)
{
	int *ptr = (int *) &vec;
	printf("%d %d %d %d %d %d %d %d\n", 
				ptr[0], ptr[1], ptr[2], ptr[3],
				ptr[4], ptr[5], ptr[6], ptr[7]);
}


int g2d_fill_triangle_boundingbox_avx (
	const int x1, const int y1, 
	const int x2, const int y2,
	const int x3, const int y3)
{

	//y's are negated bc the axis is inverted
	int
	dx12 = (x2 - x1), dy12 = (y2 - y1),
	dx23 = (x3 - x2), dy23 = (y3 - y2),
	dx31 = (x1 - x3), dy31 = (y1 - y3);

	//change the order so its counter clockwise
	if (cross2d (dx12, dy12, dx23, dy23) < 0)
	{
		return g2d_fill_triangle_boundingbox_avx (x1, y1, x3, y3, x2, y2);
	}

	int vecsize = 8;

	int min_x = max (min3 (x1, x2, x3), 0);
	int min_y = max (min3 (y1, y2, y3), 0);

	int max_x = min (max3 (x1, x2, x3), (graphics_context -> width) - 1);
	int max_y = min (max3 (y1, y2, y3), (graphics_context -> height) - 1);

	int *w1_row = malloc (sizeof(int) * vecsize); 
	int *w2_row = malloc (sizeof(int) * vecsize); 
	int *w3_row = malloc (sizeof(int) * vecsize); 

	int *w1_add_row_dat = malloc (sizeof(int) * vecsize); 
	int *w2_add_row_dat = malloc (sizeof(int) * vecsize); 
	int *w3_add_row_dat = malloc (sizeof(int) * vecsize); 

	int *w1_add_col_dat = malloc (sizeof(int) * vecsize); 
	int *w2_add_col_dat = malloc (sizeof(int) * vecsize); 
	int *w3_add_col_dat = malloc (sizeof(int) * vecsize); 

	w1_row[0] = orient2d (x2, y2, x3, y3, min_x, min_y);
	w2_row[0] = orient2d (x3, y3, x1, y1, min_x, min_y);
	w3_row[0] = orient2d (x1, y1, x2, y2, min_x, min_y);

	int w1addcol = -dy23 * vecsize;
	int w2addcol = -dy31 * vecsize;
	int w3addcol = -dy12 * vecsize;

	for (int i = 1; i < vecsize; i++)
	{
		w1_row[i] = w1_row[i-1] - dy23;
		w2_row[i] = w2_row[i-1] - dy31;
		w3_row[i] = w3_row[i-1] - dy12;
	}

	for (int i = 0; i < vecsize; i++)
	{
		w1_add_row_dat[i] = dx23;
		w1_add_col_dat[i] = w1addcol;

		w2_add_row_dat[i] = dx31;
		w2_add_col_dat[i] = w2addcol;

		w3_add_row_dat[i] = dx12;
		w3_add_col_dat[i] = w3addcol;
	}

	__m256i w1_rvec = _mm256_loadu_si256 (w1_row);
	__m256i w2_rvec = _mm256_loadu_si256 (w2_row);
	__m256i w3_rvec = _mm256_loadu_si256 (w3_row);

	__m256i w1_add_per_col = _mm256_loadu_si256 (w1_add_col_dat);
	__m256i w2_add_per_col = _mm256_loadu_si256 (w2_add_col_dat);
	__m256i w3_add_per_col = _mm256_loadu_si256 (w3_add_col_dat);

	__m256i w1_add_per_row = _mm256_loadu_si256 (w1_add_row_dat);
	__m256i w2_add_per_row = _mm256_loadu_si256 (w2_add_row_dat);
	__m256i w3_add_per_row = _mm256_loadu_si256 (w3_add_row_dat);

	short w1_row_scalar = orient2d (x2, y2, x3, y3, min_x, min_y);
	short w2_row_scalar = orient2d (x3, y3, x1, y1, min_x, min_y);
	short w3_row_scalar = orient2d (x1, y1, x2, y2, min_x, min_y);

	__m256i zero = _mm256_set1_epi32(0);
	__m256i allone = _mm256_set1_epi32(((unsigned int) -1));
	
	__m256i w1, w2, w3, allor, comp;
	for (short y = min_y; y <= max_y; y++)
	{
		w1 = w1_rvec;
		w2 = w2_rvec;
		w3 = w3_rvec;

		short w1_scalar = w1_row_scalar;
		short w2_scalar = w2_row_scalar;
		short w3_scalar = w3_row_scalar;

		for (short x = min_x; x <= max_x; x += vecsize)
		{
			allor = _mm256_set_epi32(-5, -5, -5, 1, 1, -5, -5, -5); //w1 | w2 | w3;
			comp = zero;

			//printf ("comparison: %d\n", _mm256_cmpeq_epi32 (allor, zero));


			//printf ("!0x05 = %d\n", (!0x05));
			
			//print_vec (allor);
			//print_vec (comp);
			//printf ("\n");
			//printf("%d\n\n", w1_scalar);

			w1 += w1_add_per_col;
			w2 += w2_add_per_col;
			w3 += w3_add_per_col;

			w1_scalar -= vecsize * dy23;
			w2_scalar -= vecsize * dy31;
			w3_scalar -= vecsize * dy12;
		}

		w1_rvec += w1_add_per_row;
		w2_rvec += w2_add_per_row;
		w3_rvec += w3_add_per_row;

		w1_row_scalar += dx23;
		w2_row_scalar += dx31;
		w3_row_scalar += dx12;
	}

	free (w1_row);
	free (w2_row);
	free (w3_row);
	free (w1_add_row_dat);
	free (w2_add_row_dat);
	free (w3_add_row_dat);
	free (w1_add_col_dat);
	free (w2_add_col_dat);
	free (w3_add_col_dat);
}


int g2d_fill_triangle (
	const int x1, const int y1, 
	const int x2, const int y2,
	const int x3, const int y3)
{
	g2d_set_thickness (1);

	//one point
	if (x1 == x2 && x2 == x3 && y1 == y2 && y2 == y3)
	{
		return g2d_draw_point (x1, y1);
	}

	//horizontal line
	else if (y1 == y2 && y2 == y3)
	{
		return g2d_draw_line (min3 (x1, x2, x3), y1, max3 (x1, x2, x3), y1);
	}

	//vertical line
	else if (x1 == x2 && x2 == x3)
	{
		return g2d_draw_line (x1, min3 (y1, y2, y3), x1, max3 (y1, y2, y3));
	}


	//generic triangle
	else
	{
		int topx = 0, topy = 0;
		int leftx = 0, lefty = 0;
		int rightx = 0, righty = 0;

		//order points top, left, and right
		if (y1 < y2 && y1 < y3)
		{
			topx = x1;
			topy = y1;

			leftx = x2;
			lefty = y2;
			rightx = x3;
			righty = y3;
		}
		else if (y2 < y1 && y2 < y3)
		{
			topx = x2;
			topy = y2;

			leftx = x1;
			lefty = y1;
			rightx = x3;
			righty = y3;
		}
		else if (y3 < y1 && y3 < y2)
		{
			topx = x3;
			topy = y3;

			leftx = x1;
			lefty = y1;
			rightx = x2;
			righty = y2;
		}
		
		//in these 3 cases we have a flat top, so we go from the bottom instead
		else if (y1 == y2 && y1 < y3)
		{
			topx = x3;
			topy = y3;

			leftx = x1;
			lefty = y1;
			rightx = x2;
			righty = y2;
		}

		else if (y1 == y3 && y1 < y2)
		{
			topx = x2;
			topy = y2;

			leftx = x1;
			lefty = y1;
			rightx = x3;
			righty = y3;
		}

		else if (y2 == y3 && y2 < y1)
		{
			topx = x1;
			topy = y1;

			leftx = x2;
			lefty = y2;
			rightx = x3;
			righty = y3;
		}

		//should never be the case
		else
		{
			printf ("something has gone wrong, could not find correct case for triangle fill\n");
			return 1; //something has gone wrong
		}

		if (rightx < leftx)
		{
			swap_int (&leftx, &rightx);
			swap_int (&lefty, &righty);
		}

		int min_y = min3 (y1, y2, y3);
		int max_y = max3 (y1, y2, y3);

		int myleft_x = topx;
		int myleft_y = topy;
		int left_err = 0;

		int myright_x = topx;
		int myright_y = topy;
		int right_err = 0;


		// printf ("top, left, right: (%d, %d); (%d, %d); (%d, %d)\n",
		// 	topx, topy, leftx, lefty, rightx, righty);
		bool hit_left = false, hit_right = false;

		do {
			//printf ("%d, %d; %d\n", myleft_x, myright_x, myleft_y);
			draw_horizontal_line (myleft_x, myright_x, myleft_y);

			//increment the left point until it is on the next scanline
			while (myleft_y == myright_y)
			{
				//two cases for if we are targeting the right point or the left point
				if (!hit_left)
				{
					next_line_point (topx, topy, leftx, lefty, &myleft_x, &myleft_y, &left_err);
					g2d_set_pixel (myleft_x, myleft_y, (graphics_context -> color));

					//once we get to the left point, start going towards the right point
					if (myleft_x == leftx && myleft_y == lefty)
					{
						hit_left = true;
						left_err = 0;
					}
				}
				else
				{
					if (myleft_x == rightx && myleft_y == righty)
					{
						break;
					}
					else
					{
						//increment to the next point on the line & draw the pixel
						next_line_point (leftx, lefty, rightx, righty, &myleft_x, &myleft_y, &left_err);
						g2d_set_pixel (myleft_x, myleft_y, (graphics_context -> color));
					}
				}
			}

			//increment the right point until it is on the next scanline
			while (myright_y != myleft_y)
			{
				//two cases for if we are targeting the left point or the right point
				if (!hit_right)
				{
					next_line_point (topx, topy, rightx, righty, &myright_x, &myright_y, &right_err);
					g2d_set_pixel (myright_x, myright_y, (graphics_context -> color));

					//once we get to the right point, start going towards the left point
					if (myright_x == rightx && myright_y == righty)
					{
						hit_right = true;
						right_err = 0;
					}
				}
				else
				{
					if (myright_x == leftx && myright_y == lefty)
					{
						break;
					}
					else
					{
						//increment to the next point on the line & draw the pixel
						next_line_point (rightx, righty, leftx, lefty, &myright_x, &myright_y, &right_err);
						g2d_set_pixel (myright_x, myright_y, (graphics_context -> color));
					}
				}
			}
		} 
		while ( //break when we are outside the vertical scope of the triangle
		    myleft_y > min_y && myright_y > min_y && 
			myleft_y < max_y && myright_y < max_y); 
	}

	return 0;
}