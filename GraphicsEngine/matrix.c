#include "matrix.h"

double *matrix(int nrows, int ncols) {
	return calloc(sizeof(double), nrows*ncols);
}

double *matMul(double *dest, double *mat1, int nrows1, int ncols1, double *mat2, int nrows2, int ncols2) {  // dest should be calloced
	if(ncols1 != nrows2) {
		printf("matrix does not compute, %dx%d * %dx%d\n", nrows1, ncols1, nrows2, ncols2);
		return -1;
	}
	int r, c, i;
	for(r = 0; r < nrows1; r++) {
		for(c = 0; c < ncols2; c++) {
			for(i = 0; i < nrows2; i++) {
				dest[r*ncols2+c] += mat1[r*ncols1+i] * mat2[i*ncols2+c];
			}
		}
	}
	return dest;
}

double *transpose(double *dest, double *mat, int nrows, int ncols) {
	int r, c;
	for(r = 0; r < nrows; r++) {
		for(c = 0; c < ncols; c++) {
			dest[c*nrows+r] = mat[r*ncols+c];
		}
	}
	return dest;
}

static int main() {
	// 1x2 * 2x3
	double mat1[2] = {1, 2};
	double mat2[6] = {1, 2, 3, 4, 5, 6};
	//expected result: {9, 12, 15}
	double *dest = matrix(1, 3);
	matMul(dest, mat1, 1, 2, mat2, 2, 3);
	printf("{%f %f %f}\n", dest[0], dest[1], dest[2]);
}