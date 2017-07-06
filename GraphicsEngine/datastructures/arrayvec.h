#ifndef ARRAYVEC_H
#define ARRAYVEC_H

#include "../helper.h"

typedef struct {
	void *data;
	int reserved_len;
	int used_len;
	int elem_size;
} arrayvec;

/**
 * Creates new arrayvec
 * @param spots_reserve number of spots to initially reserve
 * @param elem_size size of each element of the vec
 * @return pointer to newly created arrayvec
 */
arrayvec *av_create (int spots_reserve, size_t elem_size);

/**
 * Adds element to arrayvec at arbitrary point
 * copies data from passed location into the arrayvec's memory
 * if necessary, reallocates more space for the arrayvec
 * @param vec pointer to the relevant vector
 * @param datum pointer to where the relevant data is held
 * @param index spot where the new datum will be (0 <= index <= length)
 * @param remove toggles if the passed data should be freed at end of function
 * @return 0 on success 1 otherwise; could fail if reallocation fails
 * @precondition 0 <= index <= length
 */
int av_add (arrayvec *vec, void *datum, int index, bool remove);

/**
 * Appends element to arrayvec
 * equivalent to adding at the last spot
 * copies data from passed location into the arrayvec's memory
 * reallocates more space if necessary
 * @param vec relevent arrayvec
 * @param datum pointer to where the relevant data is held
 * @param remove toggles if the passed data should be freed at end of function
 * @return 0 on success 1 otherwise; could fail if reallocation fails
 */
int av_append (arrayvec *vec, void *datum, bool remove);

/**
 * Sets the value of the arrayvec at some index
 * @param vec relevant arrayvec
 * @param datum pointer to where the relevant data is held
 * @param index spot in arrayvec to set the data (0 <= index < length)
 * @param remove toggles if the passed data should be freed at end of function
 * @return 0 on success 1 otherwise
 */
int av_set (arrayvec *vec, void *datum, int index, bool remove);

/**
 * Removes an element from the arrayvec at the specified index
 * @precondition 0 <= index < length
 * @param vec relevant arrayvec
 * @param index spot to remove (0 <= index < length)
 * @return 0 on success 1 otherwise
 */
int av_remove (arrayvec *vec, int index);

/**
 * Removes the more recently element from the arrayvec
 * @param vec relevant arrayvec
 * @return 0 on success 1 otherwise
 * @precondition array is not empty
 */
int av_pop (arrayvec *vec);

/**
 * Returns pointer to index of arrayvec
 * @param vec relevant arrayvec
 * @param index spot to retrieve (0 <= index < length)
 * @return void pointer to requested data
 */
void *av_get (arrayvec *vec, int index);

/**
 * Returns pointer to the most recently added element of the arrayvec
 * @param vec relevant arrayvec
 * @return void pointer to most recently added element
 * @precondition array is not empty
 */
void *av_peak (arrayvec *vec);

/**
 * Clears and frees the passed arrayvec
 * @param vec arrayvec to clear
 */
void av_clear (arrayvec *vec);

/**
 * Manually reallocs more space to the arrayvec and updates reservedlen
 * @param vec arrayvec to alloc to
 * @param numElem number of elements to increase size by
 */
void av_alloc_space(arrayvec *vec, int numElem);

/**
 * Macro to create arrayvec of a certain type
 * synatitic sugar around normal creation function
 */
#define av_create_type(spots_reserve, T) \
    assert (vec->elem_size == sizeof (T)); \
    av_create (spots_reserve, sizeof (T))

/**
 * Macro to append a literal of some type to an arrayvec
 * syntatic sugar around normal append function
 * creates temporary spot in memory for the data, and passes reference
 */
#define av_append_literal(vec, datum, T) \
    assert (vec->elem_size == sizeof (T)); \
    av_append (vec, &(T){datum}, false)

/**
 * Macro to add literal of some type to some spot in arrayvec
 * syntatic sugar around normal add function
 * creates temporary spot in memory for the data, and passes reference
 */
#define av_add_literal(vec, datum, index, T) \
    assert (vec->elem_size == sizeof (T)); \
    av_add (vec, &(T){datum}, index, false)

/**
* Macro to set value in array to some literal
* syntatic sugar around normal set function
* creates temporary spot in memory for the data, and passes reference
*/
#define av_set_literal(vec, datum, index, T) { \
	av_set (vec, &(T){datum}, index, false)

/**
 * Macro to get value of some type from an arrayvec
 * syntatic sugar around normal get function
 * simply casts returned pointer to correct type
 */
#define av_get_type(vec, index, T) \
    ((T *)av_get (vec, index))

/**
 * Macro to peak last added value in arrayvec
 * syntatic sugar around normal peak function
 * simply casts returned pointer to correct type
 */
#define av_peak_type(vec, T) \
    ((T *)av_peak (vec))

/**
 * Macro to get value of some type from an arrayvec
 * syntatic sugar around normal get function
 * simply casts returned pointer to correct type and dereferences
 */
#define av_get_value(vec, index, T) \
    (*(T *)av_get (vec, index))

/**
 * Macro to peak last added value in arrayvec
 * syntatic sugar around normal peak function
 * simply casts returned pointer to correct type and dereferences
 */
#define av_peak_value(vec, T) \
    (*(T *)av_peak (vec))

/**
 * Macro to pop the most recently added value from an arrayvec and store it in
 * the passed reference
 */
#define av_pop_value(vec, ref, T) do { \
    T *ptr = av_peak_type (vec, T); \
    memcpy (ref, ptr, sizeof (T)); \
    av_pop (vec); \
} while (0)


#endif