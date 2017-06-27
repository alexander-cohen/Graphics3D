#ifndef vec2ARRAYVEC_H
#define vec2ARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "linalg/vector.h"
typedef Vec2 vec2;
struct __vec2AV {
	vec2 *data;
	int len;
	int datalen;
	void (*append) (struct __vec2AV* self, vec2 dat);
	vec2 (*pop) (struct __vec2AV* self);
	void (*replace) (struct __vec2AV* self, size_t i, vec2 dat);
	void (*replace_many) (struct __vec2AV* self, size_t i, vec2 *dats, size_t datlen);
};

typedef struct __vec2AV* vec2arrayvec;
vec2arrayvec new_vec2arrayvec();

#endif