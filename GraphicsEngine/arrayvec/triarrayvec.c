#include "triarrayvec.h"

void __triav_append(triarrayvec st, tri dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(tri));
	}
	st->data[st->len] = dat;
	st->len++;
}

tri __triav_pop(triarrayvec st) {
	assert(st->len);
	st->len--;
	return st->data[st->len];
}

void __triav_replace(triarrayvec st, size_t i, tri dat) {
	st->data[i] = dat;
}

void __triav_replacemultiple(triarrayvec st, size_t i, tri *dats, size_t lendats) {
	assert(lendats);
	if(lendats == 1) {
		return __triav_replace(st, i, dats[0]);
	}
	__triav_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__triav_append(st, dats[i]);
	}
}

triarrayvec new_triarrayvec() {
	triarrayvec res = malloc(sizeof(struct __triAV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(tri));
	res->append = __triav_append;
	res->pop = __triav_pop;
	res->replace = __triav_replace;
	res->replace_many = __triav_replacemultiple;
    return res;
}

void free_triarrayvec(triarrayvec st) {
	free(st->data);
}
