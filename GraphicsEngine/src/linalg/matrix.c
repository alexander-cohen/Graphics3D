#include "matrix.h"

matrix *empty_matrix(int nrows, int ncols) {
	matrix *ret = malloc(sizeof(matrix));
	ret->data = malloc(sizeof(double)*nrows*ncols);
	ret->rows = nrows;
	ret->cols = ncols;
	return ret;
}

matrix *matMul(matrix *dest, matrix *m1, matrix *m2) {  // dest should be calloced
	assert (m1->cols == m2->rows); //must be true to multiply

	int r, c, i;
	for(r = 0; r < m1->rows; r++) {
		for(c = 0; c < m2->cols; c++) {
			dest->data[r*m2->cols+c] = 0;
			for(i = 0; i < m2->rows; i++) {
				dest->data[r*m2->cols+c] += m1->data[r*m1->cols+i] * m2->data[i*m2->cols+c];
			}
		}
	}
	return dest;
}

matrix *transpose(matrix *dest, matrix *m) {
	int r, c;
	for(r = 0; r < m->rows; r++) {
		for(c = 0; c < m->cols; c++) {
			dest->data[c*m->rows+r] = m->data[r*m->cols+c];
		}
	}
	return dest;
}

matrix *itranspose(matrix *m) {
	//put mat into seperate space in memory, then transpose with destination of the passed matrix
	matrix *newmat = empty_matrix(m->rows, m->cols);
	memcpy (newmat->data, m->data, sizeof(double)*m->rows*m->cols);
	return transpose(m, newmat);
}

void print_matrix(matrix *m) {
	int i, j;
	printf("[");
	for(i = 0; i < m->rows; i++) {
		printf("\n\t");
		for(j = 0; j < m->cols; j++) {
			printf("%6.3f ", m->data[i*m->cols+j]);
		}
	}
	printf("\n]");
}

double matrix_normsquared(matrix *m) {
	double ret = 0;
	int i, j;
	for(i = 0; i < m->rows; i++) {
		for(j = 0; j < m->cols; j++) {
			ret += m->data[i*m->cols+j]*m->data[i*m->cols+j];
		}
	}
	return ret;
}

int mattest() {
	// 1x2 * 2x3
	matrix *m1 = empty_matrix(1, 2);
	matrix *m2 = empty_matrix(2, 3);
	memcpy(m1->data, ((double[]){1, 2}), 2*sizeof(double));
	memcpy(m2->data, ((double[]){1, 2, 3, 4, 5, 6}), 6*sizeof(double));
	//expected result: {9, 12, 15}
	matrix *dest = empty_matrix(1, 3);
	matMul(dest, m1, m2);
	printf("{%f %f %f}\n", dest->data[0], dest->data[1], dest->data[2]);

	return 0;
}