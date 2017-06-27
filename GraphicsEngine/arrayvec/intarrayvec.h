#ifndef intARRAYVEC_H
#define intARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
struct __intAV {
	int *data;
	int len;
	int datalen;
	void (*append) (struct __intAV* self, int dat);
	int (*pop) (struct __intAV* self);
	void (*replace) (struct __intAV* self, size_t i, int dat);
	void (*replace_many) (struct __intAV* self, size_t i, int *dats, size_t datlen);
};

typedef struct __intAV* intarrayvec;
intarrayvec new_intarrayvec();

#endif