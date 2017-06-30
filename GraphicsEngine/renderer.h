#pragma once
#include "primitives.h"
#include "allav.h"


typedef struct {
	vec3arrayvec vlist;
	vec3arrayvec nlist;
	vec2arrayvec tlist;
	intarrayvec  mlist;
	matarrayvec  materials;
} render_context;

typedef struct {
	int width, height;
	double *z_buffer;
	int *mat_buffer;
	int *color_buffer;
	matarrayvec materials;
} raster_context;

int *render(triangle *ts, size_t ntriangles, matarrayvec materials);