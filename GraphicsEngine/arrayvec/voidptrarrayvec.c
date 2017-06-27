#include "voidptrarrayvec.h"

void __voidptrav_append(voidptrarrayvec st, voidptr dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(voidptr));
	}
	st->data[st->len] = dat;
	st->len++;
}

voidptr __voidptrav_pop(voidptrarrayvec st) {
	assert(st->len);
	st->len--;
	return st->data[st->len];
}

void __voidptrav_replace(voidptrarrayvec st, size_t i, voidptr dat) {
	st->data[i] = dat;
}

void __voidptrav_replacemultiple(voidptrarrayvec st, size_t i, voidptr *dats, size_t lendats) {
	assert(lendats);
	if(lendats == 1) {
		return __voidptrav_replace(st, i, dats[0]);
	}
	__voidptrav_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__voidptrav_append(st, dats[i]);
	}
}

voidptrarrayvec new_voidptrarrayvec() {
	voidptrarrayvec res = malloc(sizeof(struct __voidptrAV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(voidptr));
	res->append = __voidptrav_append;
	res->pop = __voidptrav_pop;
	res->replace = __voidptrav_replace;
	res->replace_many = __voidptrav_replacemultiple;
	return res;
}

void free_voidptrarrayvec(voidptrarrayvec st) {
	free(st->data);
}
