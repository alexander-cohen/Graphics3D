#ifndef vec3ARRAYVEC_H
#define vec3ARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "linalg/vector.h"
typedef Vec3 vec3;
struct __vec3AV {
	vec3 *data;
	int len;
	int datalen;
	void (*append) (struct __vec3AV* self, vec3 dat);
	vec3 (*pop) (struct __vec3AV* self);
	void (*replace) (struct __vec3AV* self, size_t i, vec3 dat);
	void (*replace_many) (struct __vec3AV* self, size_t i, vec3 *dats, size_t datlen);
};

typedef struct __vec3AV* vec3arrayvec;
vec3arrayvec new_vec3arrayvec();

#endif