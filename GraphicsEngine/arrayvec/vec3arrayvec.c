#include "vec3arrayvec.h"

void __vec3av_append(vec3arrayvec st, vec3 dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen*sizeof(vec3));
	}
	st->data[st->len] = dat;
	st->len++;
}

vec3 __vec3av_pop(vec3arrayvec st) {
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

void __vec3av_replace(vec3arrayvec st, size_t i, vec3 dat) {
	st->data[i] = dat;
}

void __vec3av_replacemultiple(vec3arrayvec st, size_t i, vec3 *dats, size_t lendats) {
	if(lendats == 0) {
		st->data[i] = NULL;
	}
	else if(lendats == 1) {
		return __vec3av_replace(st, i, dats[0]);
	}
	__vec3av_replace(st, i, dats[0]);
	for(i = 1; i < lendats; i++) {
		__vec3av_append(st, dats[i]);
	}
}

vec3arrayvec new_vec3arrayvec() {
	vec3arrayvec res = malloc(sizeof(struct __vec3AV));
	res->len = 0;
	res->datalen = 1024;
	res->data = malloc(res->datalen*sizeof(vec3));
	res->append = __vec3av_append;
	res->pop = __vec3av_pop;
	res->replace = __vec3av_replace;
	res->replace_many = __vec3av_replacemultiple;
}

void free_vec3arrayvec(vec3arrayvec st) {
	free(st->data);
}

main() {
	vec3arrayvec a = new_vec3arrayvec();
	a->append(a, 1);
	a->append(a, 4);
	a->append(a, 9);
	a->replace(a, 1, 7); // 1 7 9
	a->replace_many(a, 0, (vec3[]){5, 4, 3}, 3); // 5 7 9 4 3
	printf("%d %d %d %d %d", a->data[0], a->data[1], a->data[2], a->data[3], a->data[4]);
}