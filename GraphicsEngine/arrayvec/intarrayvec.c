#include "intarrayvec.h"

void __intav_append(intarrayvec st, int dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(int));
	}
	st->data[st->len] = dat;
	st->len++;
}

int __intav_pop(intarrayvec st) {
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

void __intav_replace(intarrayvec st, size_t i, int dat) {
	st->data[i] = dat;
}

void __intav_replacemultiple(intarrayvec st, size_t i, int *dats, size_t lendats) {
	if(lendats == 0) {
		st->data[i] = NULL;
	}
	else if(lendats == 1) {
		return __intav_replace(st, i, dats[0]);
	}
	__intav_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__intav_append(st, dats[i]);
	}
}

intarrayvec new_intarrayvec() {
	intarrayvec res = malloc(sizeof(struct __intAV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(int));
	res->append = __intav_append;
	res->pop = __intav_pop;
	res->replace = __intav_replace;
	res->replace_many = __intav_replacemultiple;
}

void free_intarrayvec(intarrayvec st) {
	free(st->data);
}

main() {
	intarrayvec a = new_intarrayvec();
	a->append(a, 1);
	a->append(a, 4);
	a->append(a, 9);
	a->replace(a, 1, 7); // 1 7 9
	a->replace_many(a, 0, (int[]){5, 4, 3}, 3); // 5 7 9 4 3
	printf("%d %d %d %d %d", a->data[0], a->data[1], a->data[2], a->data[3], a->data[4]);
}