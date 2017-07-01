#include <stdlib.h>
#include <stdio.h>
#include "arrayvec.h"

int main () {
	arrayvec *myvec = av_create_type (1, int);

	av_append_literal (myvec, 2, int);
	av_append_literal (myvec, 8, int);
	av_append_literal (myvec, 4, int);
	av_append_literal (myvec, 6, int);

	for (int i = 0; i < myvec->used_len; i++) {
		printf ("%d\n", *av_get_type (myvec, i, int));
	}
	printf ("\n");

	av_add_literal (myvec, 37, 2, int);


	for (int i = 0; i < myvec->used_len; i++) {
		printf ("%d\n", *av_get_type (myvec, i, int));
	}
	printf ("\n");

	av_remove (myvec, 3);

	for (int i = 0; i < myvec->used_len; i++) {
		printf ("%d\n", *av_get_type (myvec, i, int));
	}
	printf ("\n");

	return 0;
}