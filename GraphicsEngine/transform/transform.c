#include "transform.h"
matrix *ident() {
	matrix *ret = empty_matrix(4, 4);
	memset(ret->data, 0, 4*4*sizeof(double)); // this works, memsetting a double with 0 bytes makes it 0
	double *d = ret->data;
	d[0] = 1;
	d[5] = 1;
	d[10] = 1;
	d[15] = 1;
	return ret;
}

matrix *chain_transforms(arrayvec *ms) {  // this is on dumb, TODO implement inplace matMul
	if(ms->used_len == 0)
		return ident();
	matrix *ret1 = empty_matrix(4, 4);
	if(ms->used_len == 1) 
		return matMul(ret1, ident(), av_get_type(ms, 0, matrix));  // why not
	matMul(ret1, av_get_type(ms, 0, matrix), av_get_type(ms, 1, matrix));	
	if(ms->used_len == 2)
		return ret1;
	matrix *ret2 = empty_matrix(4, 4);
	int i;
	// criss cross
	for(i = 2; i < ms->used_len - 1; i+=2) {
		matMul(ret2, ret1, av_get_type(ms, i, matrix));
		matMul(ret1, ret2, av_get_type(ms, i+1, matrix));
	}
	if(i < ms->used_len) {  // last increment pushed us to len-1, so we missed last index
		matMul(ret2, ret1, av_get_type(ms, i, matrix));
		return ret2;
	}
	return ret1;
}

matrix *translate(double x, double y, double z) {
	matrix *ret = ident();
	double *d = ret->data;
	d[3] = x;
	d[7] = y;
	d[11] = z;
	return ret;
}

matrix *scale(double x, double y, double z) {
	matrix *ret = ident();
	double *d = ret->data;
	d[0] = x;
	d[5] = y;
	d[10] = z;
	return ret;
}

matrix *rotate(int axis, double angle) {
	double c = cos(angle);
	double s = sin(angle);
	matrix *ret = ident();
	double *d = ret->data;
	if(axis == 2) {
		d[0] = c;
		d[1] = -s;
		d[4] = s; 
		d[5] = c;
	}
	else if(axis == 1) {
		d[5] = c;
		d[6] = -s;
		d[9] = s;
		d[10] = c;
	}
	else {
		d[0] = c;
		d[2] = s;
		d[8] = -s;
		d[10] = c;
	}
	return ret;
}

matrix *translate_inv_t(double x, double y, double z) {
	return itranspose(translate(-x, -y, -z));
}

matrix *scale_inv_t(double x, double y, double z) {
	return scale(1/x, 1/y, 1/z); // no need to transpose, S.T = S if S is a scale
}

matrix *rotate_inv_t(int axis, double angle) {
	return rotate(axis, angle);
	/* explanation: obviously, inverse of a rotation by theta is a rotation
	by -theta. sin(-theta) = -sin(theta), cos(-theta) = cos(theta)
	as can be seen in the rotate function, all the cosines are on the diagonal,
	so they don't get moved by the transpose
	and also, in every case, where there is a sin, across the diagonal, there is
	a -sin. When the rotation is inverted, these swap, and then when it is 
	transposed, they switch back. */
}

arrayvec *apply_transform_inplace(matrix *m, matrix *m_inv_t, arrayvec *tris) {
	int i;
	for(i = 0; i < tris->used_len; i++) {
		triangle *tri = av_get_type(tris, i, triangle);
		tri->p1 = transform_vec3(m, tri->p1);
		tri->p2 = transform_vec3(m, tri->p2);
		tri->p3 = transform_vec3(m, tri->p3);
		tri->n1 = transform_vec3(m_inv_t, tri->n1);
		tri->n2 = transform_vec3(m_inv_t, tri->n2);
		tri->n3 = transform_vec3(m_inv_t, tri->n3);
		double tmp = vec3norm(tri->n1);
		vec3idiv(tri->n1, tmp);
		tmp = vec3norm(tri->n2);
		vec3idiv(tri->n2, tmp);
		tmp = vec3norm(tri->n3);
		vec3idiv(tri->n3, tmp);
	}
	return tris;
}

arrayvec *apply_transform(matrix *m, matrix *m_inv_t, arrayvec *tris) {
	return apply_transform_inplace(m, m_inv_t, av_clone(tris));
}

Vec3 *transform_vec3_inplace(matrix *m, Vec3 *v) {  // XXX currently ignores w component i.e perspective shit
	double *d = m->data;
	double x = d[0] * v->x + d[1] * v->y + d[2] * v->z + d[3];
	double y = d[4] * v->x + d[5] * v->y + d[6] * v->z + d[7];
	double z = d[8] * v->x + d[9] * v->y + d[10] * v->z + d[11];
	v->x = x;
	v->y = y;
	v->z = z;
	return v;
}

Vec3 transform_vec3(matrix *m, Vec3 v) {
	double *d = m->data;
	Vec3 ret;
	ret.x = d[0] * v.x + d[1] * v.y + d[2] * v.z + d[3];
	ret.y = d[4] * v.x + d[5] * v.y + d[6] * v.z + d[7];
	ret.z = d[8] * v.x + d[9] * v.y + d[10] * v.z + d[11];
	return ret;
}

#define _r(n) (bd[i+n] = ad[n] * x + ad[n + 1] * y + ad[n + 2] * z + ad[n + 3] * w)
matrix *matmul_ip(matrix *a, matrix *b) { // ab->b, a and b 4x4
	int i;
	double x, y, z, w, *ad = a->data, *bd = b->data;
	for(i = 0; i < 4; i++) {
		x = bd[i];
		y = bd[i+4];
		z = bd[i+8];
		w = bd[i+12];
		_r(0);
		_r(4);
		_r(8);
		_r(12);
	}
	return b;
}
#undef _r

bool gluInvertMatrix(const double m[16], double invOut[16])
{
    double inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}