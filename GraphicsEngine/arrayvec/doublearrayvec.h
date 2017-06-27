#ifndef doubleARRAYVEC_H
#define doubleARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
typedef double double;
struct __doubleAV {
	double *data;
	int len;
	int datalen;
	void (*append) (struct __doubleAV* self, double dat);
	double (*pop) (struct __doubleAV* self);
	void (*replace) (struct __doubleAV* self, size_t i, double dat);
	void (*replace_many) (struct __doubleAV* self, size_t i, double *dats, size_t datlen);
};

typedef struct __doubleAV* doublearrayvec;
doublearrayvec new_doublearrayvec();

#endif