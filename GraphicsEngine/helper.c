#include "helper.h"

int swap_int (int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
    return 0;
}