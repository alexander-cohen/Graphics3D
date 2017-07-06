#pragma once
#include "primitives.h"
#include "linalg/vector.h"

typedef struct {
	arrayvec *vlist, *nlist, *tlist, *mlist, *materials;
} render_context;

typedef struct {
	int width, height;
	double *z_buffer;
	int *mat_buffer;
	int *color_buffer;
	Vec3 *n_buffer;
	arrayvec *materials;
} raster_context;

int *render(arrayvec *tris, arrayvec *materials);