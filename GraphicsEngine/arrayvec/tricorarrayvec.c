#include "tricorarrayvec.h"

void __tricorav_append(tricorarrayvec st, tricor dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(tricor));
	}
	st->data[st->len] = dat;
	st->len++;
}

tricor __tricorav_pop(tricorarrayvec st) {
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

void __tricorav_replace(tricorarrayvec st, size_t i, tricor dat) {
	st->data[i] = dat;
}

void __tricorav_replacemultiple(tricorarrayvec st, size_t i, tricor *dats, size_t lendats) {
	if(lendats == 0) {
		st->data[i] = NULL;
	}
	else if(lendats == 1) {
		return __tricorav_replace(st, i, dats[0]);
	}
	__tricorav_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__tricorav_append(st, dats[i]);
	}
}

tricorarrayvec new_tricorarrayvec() {
	tricorarrayvec res = malloc(sizeof(struct __tricorAV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(tricor));
	res->append = __tricorav_append;
	res->pop = __tricorav_pop;
	res->replace = __tricorav_replace;
	res->replace_many = __tricorav_replacemultiple;
}

void free_tricorarrayvec(tricorarrayvec st) {
	free(st->data);
}

main() {
	tricorarrayvec a = new_tricorarrayvec();
	a->append(a, 1);
	a->append(a, 4);
	a->append(a, 9);
	a->replace(a, 1, 7); // 1 7 9
	a->replace_many(a, 0, (tricor[]){5, 4, 3}, 3); // 5 7 9 4 3
	printf("%d %d %d %d %d", a->data[0], a->data[1], a->data[2], a->data[3], a->data[4]);
}