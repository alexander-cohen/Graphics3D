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

void _AV__append(arrayvec st, void * dat) {
 if(st->len >= st->datalen) {
  st->datalen *= 2;
  st->data = realloc(st->data, st->datalen*sizeof(void *));
 }
 st->data[st->len] = dat;
 st->len++;
}

void * _AV__pop(arrayvec st) {
 assert(st->len);
 st->len--;
 return st->data[st->len];
}

void _AV__replace(arrayvec st, size_t i, void * dat) {
 st->data[i] = dat;
}

void _AV__replacemultiple(arrayvec st, size_t i, void * *dats, size_t lendats) {
 assert(lendats);
 if(lendats == 1) {
  return _AV__replace(st, i, dats[0]);
 }
 _AV__replace(st, i, dats[0]);
 for(i = 1; i < lendats; i++) {
  _AV__append(st, dats[i]);
 }
}

arrayvec new_arrayvec() {
 arrayvec res = malloc(sizeof(struct _AV_));
 res->len = 0;
 res->datalen = 1024;
 res->data = malloc(res->datalen*sizeof(void *));
 res->_append = _AV__append;
 res->_pop = _AV__pop;
 res->_replace = _AV__replace;
 res->_replace_many = _AV__replacemultiple;
    return res;
}

void free_arrayvec(arrayvec st) {
 free(st->data);
}
#endif