#include "arrayvec.h"

/**
 * Create new arrayvec
 * @param spots_reserve number of spots to initially reserve
 * @param elem_size size of each element of the vec
 * @return pointer to newly created arrayvec
 */
arrayvec *av_create (int spots_reserve, size_t elem_size) {
	arrayvec *return_vec = malloc (sizeof (arrayvec));
	spots_reserve = max (spots_reserve, 1); //reserve at least 1 spot
	return_vec->data = malloc (spots_reserve * elem_size);
	return_vec->reserved_len = spots_reserve;
	return_vec->used_len = 0;
	return_vec->elem_size = elem_size;

	return return_vec;
}

int av_add (arrayvec *vec, void *datum, int index, bool remove) {
	assert (0 <= index && index <= vec->used_len);

	if (vec->used_len == vec->reserved_len) {
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

	if (remove) free (datum); //get rid of the source for this memory if the user wants
	return 0;
}

int av_append (arrayvec *vec, void *datum, bool remove) {
	return av_add (vec, datum, vec->used_len, remove);
}

int av_replace (arrayvec *vec, void *datum, int index, bool remove) {
	assert (0 <= index && index < vec->used_len);
	memcpy (av_get (vec, index), datum, vec->elem_size);
	return 0;
}

int av_remove (arrayvec *vec, int index) {
	assert (0 <= index && index < vec->used_len);

	//move everything back one onto the spot to remove
	for (int i = index; i < vec->used_len - 1; i++) {
		memcpy (av_get (vec, i), av_get (vec, i + 1), vec->elem_size);
	}

	vec->used_len--;
	return 0;
}

int av_pop (arrayvec *vec) {
	return av_remove (vec, vec->used_len-1);
}

void *av_get (arrayvec *vec, int index) {
	return (unsigned char *)vec->data + index * vec->elem_size;
}

void *av_peak (arrayvec *vec) {
	return av_get (vec, vec->used_len - 1);
}




