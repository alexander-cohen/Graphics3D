#pragma once
#include "linalg/matrix.h"
#include "datastructures/arrayvec.h"
#include "linalg/vector.h"
#include "pipeline/primitives.h"
matrix *ident(); // specifically transformation identity matrix, 4x4
matrix *chain_transforms(arrayvec *ms);
matrix *rotate(int axis, double angle); // angle in radians
matrix *translate(double x, double y, double z);
matrix *scale(double x, double y, double z);
matrix *translate_inv_t(double x, double y, double z);
matrix *scale_inv_t(double x, double y, double z);
matrix *rotate_inv_t(int axis, double angle);
arrayvec *apply_transform_inplace(matrix *m, matrix *m_inv_t, arrayvec *tris);
arrayvec *apply_transform(matrix *m, matrix *m_inv_t, arrayvec *tris);
Vec3 *transform_vec3_inplace(matrix *m, Vec3 *v);
Vec3 transform_vec3(matrix *m, Vec3 v);
matrix *matmul_ip(matrix *a, matrix *b);
bool gluInvertMatrix(const double m[16], double invOut[16]);
