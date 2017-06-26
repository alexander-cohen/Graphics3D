#include "linalg/vector.h"
#include "arrayvec/vec3arrayvec.h"
#include "arrayvec/vec2arrayvec.h"

typedef struct {
	bool hasn, hast;
	Vec3 p1, p2, p3, n1, n2, n3;
	Vec2 t1, t2, t3;
} triangle;

typedef struct {
	size_t p1, p2, p3;
} triangle_coords;

typedef struct {
	bool hasn, hast;
	vec3arrayvec vlist, nlist;
	vec2arrayvec tlist;
} VNTlist;

typedef struct {
	int placeholder;
} material;