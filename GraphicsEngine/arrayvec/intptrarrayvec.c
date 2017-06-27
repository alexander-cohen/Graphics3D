#include "intptrarrayvec.h"

void __intptrav_append(intptrarrayvec st, intptr dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(intptr));
	}
	st->data[st->len] = dat;
	st->len++;
}

intptr __intptrav_pop(intptrarrayvec st) {
	assert(st->len);
	st->len--;
	return st->data[st->len];
}

void __intptrav_replace(intptrarrayvec st, size_t i, intptr dat) {
	st->data[i] = dat;
}

void __intptrav_replacemultiple(intptrarrayvec st, size_t i, intptr *dats, size_t lendats) {
	assert(lendats);
	if(lendats == 1) {
		return __intptrav_replace(st, i, dats[0]);
	}
	__intptrav_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__intptrav_append(st, dats[i]);
	}
}

intptrarrayvec new_intptrarrayvec() {
	intptrarrayvec res = malloc(sizeof(struct __intptrAV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(intptr));
	res->append = __intptrav_append;
	res->pop = __intptrav_pop;
	res->replace = __intptrav_replace;
	res->replace_many = __intptrav_replacemultiple;
	return res;
}

void free_intptrarrayvec(intptrarrayvec st) {
	free(st->data);
}
