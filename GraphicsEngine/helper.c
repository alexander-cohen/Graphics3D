#include "helper.h"

inline int swap_int (int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}