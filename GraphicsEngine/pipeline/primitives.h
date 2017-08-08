#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "linalg/vector.h"
#include "datastructures/arrayvec.h"
#include <stdbool.h>

typedef struct {
	size_t p1, p2, p3;
} triangle_coords;

typedef struct {
	bool hasn, hast;
	arrayvec vlist, nlist, tlist;
} VNTlist;

typedef struct {
	Vec3 *color_data; // each Vec3 should be an RGB triplet, each channel 0.0-1.0 for 0-255
	int width, height;
} texture;

typedef struct {
	Vec3 Ka, Kd, Ks;
	texture Ka_map, Kd_map, Ks_map;
	int shininess;
	int color; // XXX useless holdover, destroy
} material;

typedef struct {
	Vec3 pos, Ia, Id, Is;
} light;

typedef struct {
	bool hasn, hast;
	Vec3 p1, p2, p3, n1, n2, n3;
	Vec2 t1, t2, t3;
	int mat;
} triangle;

#endif