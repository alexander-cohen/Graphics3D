#ifndef AV2_H
#define AV2_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define append(av, type, data) (av->_append(av, &data))
#define pop(av, type) (*(type *)av->_pop(av))
#define set(av, type, i, data) (av->_replace(av, i, &data))
#define get(av, type, i) (*(type *)av->data[i])
// for setptr type is unused, but it fits
#define setptr(av, type, i, ptr) (av->data[i] = ptr)
#define getptr(av, type, i) ((type *)av->data[i])
#define appendptr(av, type, ptr) (av->_append(av, ptr))

struct _AV_ {
 void * *data;
 int len;
 int datalen;
 void (*_append) (struct _AV_ *self, void * dat);
 void * (*_pop) (struct _AV_ *self);
 void (*_replace) (struct _AV_ *self, size_t i, void * dat);
 void (*_replace_many) (struct _AV_ *self, size_t i, void * *dats, size_t datlen);
};

typedef struct _AV_* arrayvec;
arrayvec new_arrayvec();

#endif