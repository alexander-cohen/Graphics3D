#pragma once
#include "primitives.h"


typedef struct {
	arrayvec *vlist, *nlist, *tlist, *mlist, *materials;
} render_context;

typedef struct {
	int width, height;
	double *z_buffer;
	int *mat_buffer;
	int *color_buffer;
	arrayvec *materials;
} raster_context;

int *render(arrayvec *tris, size_t ntriangles, arrayvec *materials);