#include "doublearrayvec.h"

void __doubleav_append(doublearrayvec st, double dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(double));
	}
	st->data[st->len] = dat;
	st->len++;
}

double __doubleav_pop(doublearrayvec st) {
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

void __doubleav_replace(doublearrayvec st, size_t i, double dat) {
	st->data[i] = dat;
}

void __doubleav_replacemultiple(doublearrayvec st, size_t i, double *dats, size_t lendats) {
	if(lendats == 0) {
		st->data[i] = NULL;
	}
	else if(lendats == 1) {
		return __doubleav_replace(st, i, dats[0]);
	}
	__doubleav_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__doubleav_append(st, dats[i]);
	}
}

doublearrayvec new_doublearrayvec() {
	doublearrayvec res = malloc(sizeof(struct __doubleAV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(double));
	res->append = __doubleav_append;
	res->pop = __doubleav_pop;
	res->replace = __doubleav_replace;
	res->replace_many = __doubleav_replacemultiple;
}

void free_doublearrayvec(doublearrayvec st) {
	free(st->data);
}

main() {
	doublearrayvec a = new_doublearrayvec();
	a->append(a, 1);
	a->append(a, 4);
	a->append(a, 9);
	a->replace(a, 1, 7); // 1 7 9
	a->replace_many(a, 0, (double[]){5, 4, 3}, 3); // 5 7 9 4 3
	printf("%d %d %d %d %d", a->data[0], a->data[1], a->data[2], a->data[3], a->data[4]);
}