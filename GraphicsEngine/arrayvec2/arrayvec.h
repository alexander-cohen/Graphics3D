#ifndef ARRAYVEC_H
#define ARRAYVEC_H

#include "../helper.h"

typedef struct {
	void *data;
	int reserved_len;
	int used_len;
	int elem_size;
} arrayvec;

arrayvec *av_create (int spots_reserve, size_t elem_size);

int av_append (arrayvec *vec, void *datum, bool remove);
int av_add (arrayvec *vec, void *datum, int index, bool remove);
int av_replace (arrayvec *vec, void *datum, int index, bool remove);
int av_remove (arrayvec *vec, int index);
int av_pop (arrayvec *vec);

void *av_get (arrayvec *vec, int index);
void *av_peak (arrayvec *vec);

#define av_create_type(spots_reserve, T) av_create (spots_reserve, sizeof (T))

#define av_append_literal(vec, datum, T) av_add_literal(vec, datum, vec->used_len, T);

#define av_add_literal(vec, datum, index, T) { \
	assert (sizeof (T) == vec->elem_size); \
	T *datum_ptr = malloc (sizeof (T)); \
	*datum_ptr = datum; \
	av_add (vec, datum_ptr, index, true); \
}

#define av_replace_literal(vec, datum, index, T) { \
	assert (sizeof (T) == vec->elem_size); \
	T *datum_ptr = malloc (sizeof (T)); \
	*datum_ptr = datum; \
	av_replace (vec, datum_ptr, index, true); \
}

//these act as l-values bc they are macros not functions
#define av_get_type(vec, index, T) (T *)av_get (vec, index)
#define av_peak_type(vec, T) (T *)av_peak (vec)

#endif