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
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

void __voidptrav_replace(voidptrarrayvec st, size_t i, voidptr dat) {
	st->data[i] = dat;
}

void __voidptrav_replacemultiple(voidptrarrayvec st, size_t i, voidptr *dats, size_t lendats) {
	if(lendats == 0) {
		st->data[i] = NULL;
	}
	else if(lendats == 1) {
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
}

void free_voidptrarrayvec(voidptrarrayvec st) {
	free(st->data);
}

main() {
	voidptrarrayvec a = new_voidptrarrayvec();
	a->append(a, 1);
	a->append(a, 4);
	a->append(a, 9);
	a->replace(a, 1, 7); // 1 7 9
	a->replace_many(a, 0, (voidptr[]){5, 4, 3}, 3); // 5 7 9 4 3
	printf("%d %d %d %d %d", a->data[0], a->data[1], a->data[2], a->data[3], a->data[4]);
}