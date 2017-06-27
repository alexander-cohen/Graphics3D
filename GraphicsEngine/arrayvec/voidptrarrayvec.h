#ifndef voidptrARRAYVEC_H
#define voidptrARRAYVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef void * voidptr;
struct __voidptrAV {
	voidptr *data;
	int len;
	int datalen;
	void (*append) (struct __voidptrAV* self, voidptr dat);
	voidptr (*pop) (struct __voidptrAV* self);
	void (*replace) (struct __voidptrAV* self, size_t i, voidptr dat);
	void (*replace_many) (struct __voidptrAV* self, size_t i, voidptr *dats, size_t datlen);
};

typedef struct __voidptrAV* voidptrarrayvec;
voidptrarrayvec new_voidptrarrayvec();

#endif