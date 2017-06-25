#include "matrix.h"

double *matrix(int nrows, int ncols) {
	return calloc(sizeof(double), nrows*ncols);
}

double *matMul(double *dest, double *mat1, int nrows1, int ncols1, double *mat2, int nrows2, int ncols2) {  // dest should be calloced
	assert (ncols1 == nrows2); //must be true to multiply

	int r, c, i;
	for(r = 0; r < nrows1; r++) {
		for(c = 0; c < ncols2; c++) {
			dest[r*ncols2+c] = 0;
			for(i = 0; i < nrows2; i++) {
				dest[r*ncols2+c] += mat1[r*ncols1+i] * mat2[i*ncols2+c];
			}
		}
	}
	return dest;
}

double *transpose(double *dest, double *mat, const int nrows, const int ncols) {
	int r, c;
	for(r = 0; r < nrows; r++) {
		for(c = 0; c < ncols; c++) {
			dest[c*nrows+r] = mat[r*ncols+c];
		}
	}
	return dest;
}

double *itranspose(double *mat, const int nrows, const int ncols) {
	//put mat into seperate space in memory, then transpose with destination of the passed matrix
	int size = sizeof (double) * nrows * ncols;
	double *newmat = (double *) malloc (size);
	memcpy (newmat, mat, size);
	transpose (mat, newmat, nrows, ncols);
	return mat;
}

int mattest() {
	// 1x2 * 2x3
	double mat1[2] = {1, 2};
	double mat2[6] = {1, 2, 3, 4, 5, 6};
	//expected result: {9, 12, 15}
	double *dest = matrix(1, 3);
	matMul(dest, mat1, 1, 2, mat2, 2, 3);
	printf("{%f %f %f}\n", dest[0], dest[1], dest[2]);

	return 0;
}