#ifndef matARRAYVEC_H
#define matARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "primitives.h"

typedef material mat;
struct __matAV {
	mat *data;
	int len;
	int datalen;
	void (*append) (struct __matAV* self, mat dat);
	mat (*pop) (struct __matAV* self);
	void (*replace) (struct __matAV* self, size_t i, mat dat);
	void (*replace_many) (struct __matAV* self, size_t i, mat *dats, size_t datlen);
};

typedef struct __matAV* matarrayvec;
matarrayvec new_matarrayvec();

#endif