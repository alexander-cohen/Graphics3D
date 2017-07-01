#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "linalg/vector.h"
#include "av2.h"
#include <stdbool.h>

typedef struct {
	size_t p1, p2, p3;
} triangle_coords;

typedef struct {
	bool hasn, hast;
	arrayvec vlist, nlist, tlist;
} VNTlist;

typedef struct {
	int color;
} material;

typedef struct {
	bool hasn, hast;
	Vec3 p1, p2, p3, n1, n2, n3;
	Vec2 t1, t2, t3;
	int mat;
} triangle;

#endif