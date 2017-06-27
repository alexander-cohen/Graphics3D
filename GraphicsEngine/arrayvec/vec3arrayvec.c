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
	assert(st->len);
	st->len--;
	return st->data[st->len];
}

void __vec3av_replace(vec3arrayvec st, size_t i, vec3 dat) {
	st->data[i] = dat;
}

void __vec3av_replacemultiple(vec3arrayvec st, size_t i, vec3 *dats, size_t lendats) {
	assert(lendats);
	if(lendats == 1) {
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
	return res;
}

void free_vec3arrayvec(vec3arrayvec st) {
	free(st->data);
}
