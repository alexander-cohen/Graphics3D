#include <stdlib.h>
#include <stdio.h>

double *matrix(int nrows, int ncols);

double *matMul(double *dest, double *mat1, int nrows1, int ncols1, double *mat2, int nrows2, int ncols2);

double *transpose(double *dest, double *mat, int nrows, int ncols);