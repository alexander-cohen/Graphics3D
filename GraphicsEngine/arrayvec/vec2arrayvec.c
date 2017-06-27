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
	assert(st->len);
	st->len--;
	return st->data[st->len];
}

void __vec2av_replace(vec2arrayvec st, size_t i, vec2 dat) {
	st->data[i] = dat;
}

void __vec2av_replacemultiple(vec2arrayvec st, size_t i, vec2 *dats, size_t lendats) {
	assert(lendats);
	if(lendats == 1) {
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
	return res;
}

void free_vec2arrayvec(vec2arrayvec st) {
	free(st->data);
}
