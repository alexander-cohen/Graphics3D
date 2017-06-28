#ifndef triARRAYVEC_H
#define triARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "primitives.h"
typedef triangle tri;
struct __triAV {
	tri *data;
	int len;
	int datalen;
	void (*append) (struct __triAV* self, tri dat);
	tri (*pop) (struct __triAV* self);
	void (*replace) (struct __triAV* self, size_t i, tri dat);
	void (*replace_many) (struct __triAV* self, size_t i, tri *dats, size_t datlen);
};

typedef struct __triAV* triarrayvec;
triarrayvec new_triarrayvec();

#endif