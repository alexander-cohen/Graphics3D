#include "vec2arrayvec.h"

void __vec2av_append(vec2arrayvec st, vec2 dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(vec2));
	}
	st->data[st->len] = dat;
	st->len++;
}

vec2 __vec2av_pop(vec2arrayvec st) {
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

void __vec2av_replace(vec2arrayvec st, size_t i, vec2 dat) {
	st->data[i] = dat;
}

void __vec2av_replacemultiple(vec2arrayvec st, size_t i, vec2 *dats, size_t lendats) {
	if(lendats == 0) {
		st->data[i] = NULL;
	}
	else if(lendats == 1) {
		return __vec2av_replace(st, i, dats[0]);
	}
	__vec2av_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__vec2av_append(st, dats[i]);
	}
}

vec2arrayvec new_vec2arrayvec() {
	vec2arrayvec res = malloc(sizeof(struct __vec2AV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(vec2));
	res->append = __vec2av_append;
	res->pop = __vec2av_pop;
	res->replace = __vec2av_replace;
	res->replace_many = __vec2av_replacemultiple;
}

void free_vec2arrayvec(vec2arrayvec st) {
	free(st->data);
}

main() {
	vec2arrayvec a = new_vec2arrayvec();
	a->append(a, 1);
	a->append(a, 4);
	a->append(a, 9);
	a->replace(a, 1, 7); // 1 7 9
	a->replace_many(a, 0, (vec2[]){5, 4, 3}, 3); // 5 7 9 4 3
	printf("%d %d %d %d %d", a->data[0], a->data[1], a->data[2], a->data[3], a->data[4]);
}