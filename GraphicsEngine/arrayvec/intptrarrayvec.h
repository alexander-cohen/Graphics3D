#ifndef intptrARRAYVEC_H
#define intptrARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef int * intptr;
struct __intptrAV {
	intptr *data;
	int len;
	int datalen;
	void (*append) (struct __intptrAV* self, intptr dat);
	intptr (*pop) (struct __intptrAV* self);
	void (*replace) (struct __intptrAV* self, size_t i, intptr dat);
	void (*replace_many) (struct __intptrAV* self, size_t i, intptr *dats, size_t datlen);
};

typedef struct __intptrAV* intptrarrayvec;
intptrarrayvec new_intptrarrayvec();

#endif