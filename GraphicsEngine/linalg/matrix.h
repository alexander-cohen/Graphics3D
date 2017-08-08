#ifndef MATRIX_H
#define MATRIX_H

#include "helper.h"

typedef struct {
	double *data;
	int rows, cols;
} matrix;

matrix *empty_matrix(int nrows, int ncols);
matrix *matMul(matrix *dest, matrix *m1, matrix *m2);
matrix *transpose(matrix *dest, matrix *m);
matrix *itranspose(matrix *m);
void print_matrix(matrix *m);
double matrix_normsquared(matrix *m);
int mattest();

#endif