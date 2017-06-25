#include <stdio.h>
#include <stdlib.h>

struct __csstack {
	double **data;
	int len;
	int datalen;
	void (*push) (struct __csstack*, double *dat);
	double *(*pop) (struct __csstack*);
};

typedef struct __csstack* stack;