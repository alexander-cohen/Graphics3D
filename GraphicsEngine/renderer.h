#pragma once
#include "primitives.h"
#include "arrayvec/intarrayvec.h"
#include "arrayvec/matarrayvec.h"
//#include "input_assembler.h"
//#include "vertex_shader.h"
//#include "tessellation_shader.h"
//#include "geometry_shader.h"
//#include "clipper.h"
//#include "fragment_shader.h"


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