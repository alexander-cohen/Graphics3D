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
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

void __matav_replace(matarrayvec st, size_t i, mat dat) {
	st->data[i] = dat;
}

void __matav_replacemultiple(matarrayvec st, size_t i, mat *dats, size_t lendats) {
	if(lendats == 0) {
		st->data[i] = NULL;
	}
	else if(lendats == 1) {
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
}

void free_matarrayvec(matarrayvec st) {
	free(st->data);
}

main() {
	matarrayvec a = new_matarrayvec();
	a->append(a, 1);
	a->append(a, 4);
	a->append(a, 9);
	a->replace(a, 1, 7); // 1 7 9
	a->replace_many(a, 0, (mat[]){5, 4, 3}, 3); // 5 7 9 4 3
	printf("%d %d %d %d %d", a->data[0], a->data[1], a->data[2], a->data[3], a->data[4]);
}