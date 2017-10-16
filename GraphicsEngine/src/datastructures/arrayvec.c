#include "arrayvec.h"

arrayvec *av_create (int spots_reserve, size_t elem_size) {
	arrayvec *return_vec = malloc (sizeof (arrayvec));
	spots_reserve = max (spots_reserve, 1); //reserve at least 1 spot
	return_vec->data = malloc (spots_reserve * elem_size);
	return_vec->reserved_len = spots_reserve;
	return_vec->used_len = 0;
	return_vec->elem_size = elem_size;

	return return_vec;
}

void vec_init_space (arrayvec *vec) {
	vec->used_len = 0;
	vec->reserved_len = 1;
	vec->data = malloc (vec->reserved_len * vec->elem_size);
}

void av_alloc_space(arrayvec *vec, int numElem) {
	vec->reserved_len += numElem;
	vec->data = realloc(vec->data, vec->elem_size * (vec->reserved_len));
}

arrayvec *av_clone(arrayvec *vec) {
	arrayvec *ret = av_create(vec->used_len, vec->elem_size);
	av_concat(ret, vec);
	return ret;
}

int av_add (arrayvec *vec, void *datum, int index, bool remove) {
	assert (0 <= index && index <= vec->used_len);

	if (vec->reserved_len == 0) {
		vec_init_space (vec);
	}
	else if (vec->used_len == vec->reserved_len) {
		vec->reserved_len *= 2;
		vec->data = realloc (vec->data, vec->elem_size * (vec->reserved_len));
		if (vec->data == NULL) { //error reallocating memory
			return 1;
		}
	}

	//shift everything back one spot to make room for new data
	for (int i = vec->used_len; i > index; i--) {
		memcpy (av_get (vec, i), av_get (vec, i - 1), vec->elem_size);
	}

	//put relevant data in correct spot
	memcpy (av_get (vec, index), datum, vec->elem_size);
	vec->used_len++;

	//get rid of the source for this memory if the user wants
	if (remove) free (datum); 
	
	return 0;
}

int av_append (arrayvec *vec, void *datum, bool remove) {
	return av_add (vec, datum, vec->used_len, remove);
}

int av_set (arrayvec *vec, void *datum, int index, bool remove) {
	assert (0 <= index && index < vec->used_len);
	memcpy (av_get (vec, index), datum, vec->elem_size);
	return 0;
}

int av_fill(arrayvec *vec, void *datum, int num) {
	vec->used_len = 0;
	int i = 0;
	for(i = 0; i < num; i++) {
		if(av_append(vec, datum, false))
			return 1;
	}
	return 0;
}

int av_concat(arrayvec *vec, arrayvec *other) {
	assert(vec->elem_size == other->elem_size);
	av_alloc_space(vec, other->used_len);
	memcpy(vec->data + vec->elem_size * vec->used_len, other->data, vec->elem_size * other->used_len);
	vec->used_len += other->used_len;
	return 0;
}

int av_remove (arrayvec *vec, int index) {
	assert (0 <= index && index < vec->used_len);
	memcpy(av_get(vec, index), av_get(vec, index + 1), vec->elem_size * (vec->used_len - index - 1));
	vec->used_len--;
	return 0;
}

int av_pop (arrayvec *vec) {
	if(vec->used_len) {
		vec->used_len--;
		return 0;
	}
	else
		return 1;
}

void *av_get (arrayvec *vec, int index) {
	return (unsigned char *)vec->data + index * vec->elem_size;
}

void *av_peak (arrayvec *vec) {
	return av_get (vec, vec->used_len - 1);
}

void av_clear (arrayvec *vec) {
	vec->used_len = 0;
	vec->reserved_len = 0;
	free (vec->data);
}

void *av_next(arrayvec *vec, int *ptr) {
	if(*ptr >= vec->used_len) return NULL;
	return av_get(vec, (*ptr)++);
}