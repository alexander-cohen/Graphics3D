#include "matarrayvec.h"

void __matav_append(matarrayvec st, mat dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(mat));
	}
	st->data[st->len] = dat;
	st->len++;
}

mat __matav_pop(matarrayvec st) {
	assert(st->len);
	st->len--;
	return st->data[st->len];
}

void __matav_replace(matarrayvec st, size_t i, mat dat) {
	st->data[i] = dat;
}

void __matav_replacemultiple(matarrayvec st, size_t i, mat *dats, size_t lendats) {
	assert(lendats);
	if(lendats == 1) {
		return __matav_replace(st, i, dats[0]);
	}
	__matav_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__matav_append(st, dats[i]);
	}
}

matarrayvec new_matarrayvec() {
	matarrayvec res = malloc(sizeof(struct __matAV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(mat));
	res->append = __matav_append;
	res->pop = __matav_pop;
	res->replace = __matav_replace;
	res->replace_many = __matav_replacemultiple;
    return res;
}

void free_matarrayvec(matarrayvec st) {
	free(st->data);
}
