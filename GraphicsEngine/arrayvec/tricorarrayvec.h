#ifndef tricorARRAYVEC_H
#define tricorARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include "primitives.h"

typedef triangle_coords tricor;
struct __tricorAV {
	tricor *data;
	int len;
	int datalen;
	void (*append) (struct __tricorAV* self, tricor dat);
	tricor (*pop) (struct __tricorAV* self);
	void (*replace) (struct __tricorAV* self, size_t i, tricor dat);
	void (*replace_many) (struct __tricorAV* self, size_t i, tricor *dats, size_t datlen);
};

typedef struct __tricorAV* tricorarrayvec;

#endif