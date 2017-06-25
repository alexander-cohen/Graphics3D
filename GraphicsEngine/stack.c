#include "stack.h"

void __stack_push(stack st, double *dat) {
	if(st->len >= st->datalen) {
		st->datalen *= 2;
		st->data = realloc(st->data, st->datalen);
	}
	st->data[st->len] = dat;
	st->len++;
}

double *__stack_pop(stack st) {
	if(!st->len)
		return NULL;
	st->len--;
	return st->data[st->len];
}

stack new_stack() {
	stack res = malloc(sizeof(struct __csstack));
	res->len = 0;
	res->datalen = 4;
	res->data = malloc(4*sizeof(void *));
	res->push = __stack_push;
	res->pop = __stack_pop;
}
