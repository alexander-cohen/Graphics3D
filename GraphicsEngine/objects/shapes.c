//
// Created by gmarks on 7/6/17.
//


#include "shapes.h"

// arrayvec *ptsTrisToTriangles(arrayvec *pts, arrayvec *tris, int mat) {  // pts is vec3, tris is ints
//     int i3;
//     triangle t;
//     arrayvec *ret = av_create(tris->used_len/3, sizeof(triangle));
//     for(i3 = 0; i3 < tris->used_len; i3+=3) {
//         t.hasn = false;
//         t.hast = false;
//         t.p1 = av_get_value(pts, av_get_value(tris, i3, int), Vec3);
//         printf("in sph gen: p1 at (%f %f %f)\n", t.p1.x, t.p1.y, t.p1.z);
//         t.p2 = av_get_value(pts, av_get_value(tris, i3+1, int), Vec3);
//         t.p3 = av_get_value(pts, av_get_value(tris, i3+2, int), Vec3);
//         t.mat = mat;
//         av_append(ret, &t, false);
//     }
//     return ret;
// }

arrayvec *spherePoints(double x, double y, double z, double radius, int steps) {
    steps = steps / 2 * 2; // even number of steps ensures poles on bottom and top
    arrayvec *pts = av_create(steps * (steps + 1) + 1, sizeof(Vec3));
    int i, j, idx = 0;
    double theta, phi, rsp;
    Vec3 pole0 = (Vec3) {x, y, z+radius};
    for(i = 0; i < steps; i++) {
        for(j = 0; j < steps; j++) {
            theta = i * 2 * M_PI / steps;
            phi = j * M_PI / steps;  // only goes around half the circle
            rsp = radius * sin(phi);
            //printf("Generated point %d (%f %f %f)\n", idx, rsp*cos(theta), rsp*sin(theta), radius * cos(phi));
            idx++;
            av_append(pts, &(Vec3){x+rsp*cos(theta), y+rsp*sin(theta), z+radius * cos(phi)}, false);

        }
    }
    Vec3 ptx = av_get_value(pts, 3, Vec3);
    printf("sample point gen'd: #3 is at (%f %f %f)\n", ptx.x, ptx.y, ptx.z);
    Vec3 *data_end = pts->data + sizeof(Vec3) * steps * steps;
    memcpy(data_end, pts->data, sizeof(Vec3) * steps); // copy first circle to end to make it easier to triangle up
    pts->used_len += steps; // super shady
    av_append(pts, &pole0, false);
    return pts;
}

arrayvec *sphereTris(int steps) {  // returns arrayvec of ints corresponding to point indices
    /* alg: making two triangles between every square of points. i.e. the first square
     * is 1, 2, steps+1, steps + 2, so tris are (1, 2, s+1), (2, s+1, s+2)
     * orientation will be fucky, we have to fix
     * special cases should be covered by the fuckiness we did above
     */
    steps = steps / 2 * 2;
    arrayvec *tris = av_create(steps * steps * 6, sizeof(int));
    int iN, iTN, iT, i, j, idx = 0;
    for(i = 0; i < steps; i++) {
        for(j = 0; j < steps; j++, idx++) {
            iN = idx + 1;
            iTN = idx + steps;
            iT = iTN++;
            if(j == steps-1) { // fuck fuck guy things to fix the fucky shit we did above
                av_append(tris, &idx, false);
                av_append(tris, &iT, false);
                av_append(tris, &iN, false);
                av_append(tris, &iN, false);
                av_append(tris, &iT, false);
                av_append(tris, &iTN, false);
            }
            else {
                av_append(tris, &idx, false);
                av_append(tris, &iN, false);
                av_append(tris, &iT, false);
                av_append(tris, &iN, false);
                av_append(tris, &iTN, false);
                av_append(tris, &iT, false);
            }
        }
    }
    return tris;
}

arrayvec *boxPoints(double x, double y, double z, double width, double height, double depth) { // goes from x, y, z, to x+w,y+h,z+d
    if(width < 0) {
        x += width;
        width = -width;
    }
    if(height < 0) {
        y += height;
        height = -height;
    }
    if(depth < 0) {
        z += depth;
        depth = -depth;
    }
    arrayvec *pts = av_create(8, sizeof(Vec3));
    int xi, yi, zi;
    for(zi = 0; zi < 2; zi++) {
        for(yi = 0; yi < 2; yi++) {
            for(xi = 0; xi < 2; xi++) {
                av_append(pts, &(Vec3){x+xi*width, y+yi*height, z+zi*depth}, false);
            }
        }
    }
    return pts;
}

#define tri(a, b, c) \
av_append_literal(tris, a, int); \
av_append_literal(tris, b, int); \
av_append_literal(tris, c, int)
arrayvec *boxTris() {  // returns arrayvec of ints corresponding to point indices
    arrayvec *tris = av_create(36, sizeof(int));
    tri(0, 1, 2); // back
    tri(1, 3, 2);
    tri(0, 6, 2); // left
    tri(0, 4, 6);
    tri(0, 4, 1); // top
    tri(1, 4, 5);
    tri(1, 5, 3); // right
    tri(3, 5, 7);
    tri(2, 3, 6); // bottom
    tri(6, 3, 7);
    tri(4, 6, 5); // front
    tri(5, 6, 7);
    return tris;
}
#undef tri

arrayvec *torusPoints(double x, double y, double z, double outerRadius, double innerRadius, int outerSteps, int innerSteps) { // makes a torus with outer circle in the XY plane
    // equation: (before translation by (x,y,z))
    // for center point of inner circle: x = outer*cos(theta), y = outer*sin(theta), z = 0
    // for point on inner circle ignoring translation and rotation: x = inner*cos(phi), y = 0, z = inner*sin(phi)
    // rotation around z axis by theta: (x, y, z) -> (f(x, y), g(x, y), z)
    // let r = |(x, y)|, f(x, y) = r*cos(cos-1(x/r)+theta), g(x, y) = r*sin(cos-1(x/r)+theta)
    // cos(a+b) = cos(a)cos(b)-sin(a)sin(b)
    // sin(a+b) = sin(a)cos(b)+cos(a)sin(b)
    // cos(cos-1(x/r)+theta) = x/r*cos(theta) - y/r*sin(theta)
    // sin(cos-1(x/r)+theta) = y/r*cos(theta) + x/r*sin(theta)
    // f(x, y) = x*cos(theta) - y*sin(theta)
    // g(x, y) = x*sin(theta) + y*cos(theta)
    // total equation = (rotation around z axis by theta)(equation for unrotated inner circle) + (equation for center point for inner circle) + (x, y, z)
    // rotation(inner*cos(phi), 0, inner*sin(phi)) = (inner*cos(phi)*cos(theta), inner*cos(phi)*sin(theta), inner*sin(phi))
    // so calculate eq for center point of inner circle in outer loop, then calculate above each iteration of inner loop
    arrayvec *pts = av_create(innerSteps * (outerSteps + 1) + 1, sizeof(Vec3));
    int i, j, idx = 0;
    double theta, phi, rsp, ct, st, ist, ict, bx, by, bz, cp;
    for(i = 0; i < outerSteps; i++) {
        theta = i * 2 * M_PI / outerSteps;
        ct = cos(theta);
        st = sin(theta);
        ict = innerRadius*ct;
        ist = innerRadius*st;
        bx = x + outerRadius*ct;
        by = y + outerRadius*st;
        bz = z;
        for(j = 0; j < innerSteps; j++) {
            phi = j * 2 * M_PI / innerSteps;
            cp = cos(phi);
            //printf("Generated point %d (%f %f %f)\n", idx, rsp*cos(theta), rsp*sin(theta), radius * cos(phi));
            idx++;
            av_append(pts, &(Vec3){bx+ict*cp, by+ist*cp, bz+innerRadius*sin(phi)}, false);

        }
    }
    Vec3 ptx = av_get_value(pts, 3, Vec3);
    printf("sample point gen'd: #3 is at (%f %f %f)\n", ptx.x, ptx.y, ptx.z);
    Vec3 *data_end = pts->data + sizeof(Vec3) * innerSteps * outerSteps;
    memcpy(data_end, pts->data, sizeof(Vec3) * innerSteps); // copy first circle to end to make it easier to triangle up
    pts->used_len += innerSteps; // super shady
    data_end += innerSteps;
    memcpy(data_end, pts->data, sizeof(Vec3));
    return pts;
}

arrayvec *torusTris(int outerSteps, int innerSteps) {  // returns arrayvec of ints corresponding to point indices
    /* alg: making two triangles between every square of points. i.e. the first square
     * is 1, 2, steps+1, steps + 2, so tris are (1, 2, s+1), (2, s+1, s+2)
     * orientation will be fucky, we have to fix
     * special cases should be covered by the fuckiness we did above
     */
    arrayvec *tris = av_create(outerSteps * innerSteps * 6, sizeof(int));
    int iN, iTN, iT, i, j, idx = 0;
    for(i = 0; i < outerSteps; i++) {
        for(j = 0; j < innerSteps; j++, idx++) {
            iN = idx + 1;
            iTN = idx + outerSteps;
            iT = iTN++;
            if(j == innerSteps-1) { // fuck fuck guy things to fix the fucky shit we did above
                av_append(tris, &idx, false);
                av_append(tris, &iT, false);
                av_append(tris, &iN, false);
                av_append(tris, &iN, false);
                av_append(tris, &iT, false);
                av_append(tris, &iTN, false);
            }
            else {
                av_append(tris, &idx, false);
                av_append(tris, &iN, false);
                av_append(tris, &iT, false);
                av_append(tris, &iN, false);
                av_append(tris, &iTN, false);
                av_append(tris, &iT, false);
            }
        }
    }
    return tris;
}

void check_orient(arrayvec *vxs, arrayvec *norms) {  // checks if all triangles with z > 0 are positively oriented and vice versa
    int i;
    Vec3 vx, norm;
    for(i = 0; i < vxs->used_len; i++) {
        vx = av_get_value(vxs, i, Vec3);
        norm = av_get_value(norms, i, Vec3);
        if((vx.z >= 0) != (norm.z >= 0)) {
            printf("Mismatch between vertex location and norm orientation detected\n");
            printf("Point %d: (%f, %f, %f)\nNorm %d: (%f, %f, %f)\n", 
                i, vx.x, vx.y, vx.z, i, norm.x, norm.y, norm.z);
        }
    }
}


unsigned int hashint(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}


int hashLong3Tuple(void *l3t) {
    int *p = l3t;
    int value = 0x345678;
    value = (1000003 * value) ^ hashint(p[0]);
    value = (1000003 * value) ^ hashint(p[1]);
    value = (1000003 * value) ^ hashint(p[2]);
    value = (1000003 * value) ^ hashint(p[3]);
    value = (1000003 * value) ^ hashint(p[4]);
    value = (1000003 * value) ^ hashint(p[5]);
    value = value ^ 6;
    if(value == -1)
        value = -2;
    return value;
}


bool equalsLong3(void *a, void *b) {
    long *pa = a;
    long *pb = b;
    return pa[0] == pb[0] && pa[1] == pb[1] && pa[2] == pb[2];
}


void fix_overlap(arrayvec *vxs, arrayvec *tri_idxs) {
    Hashmap *vmap = hashmapCreate(tri_idxs->used_len, &hashLong3Tuple, &equalsLong3);
    int i, j, one = 1, zero = 0;
    Vec3 *real;
    int *out = malloc(sizeof(int *));
    long *key = malloc(sizeof(long) * 3), *okey = malloc(sizeof(long) * 3);
    assert(sizeof(long) == sizeof(double));
    for(i = 0; i < tri_idxs->used_len; i++) {
        //printf("ITERATION %d\n", i);
        int *ind = av_get_type(tri_idxs, i, int);
        //if(*ind == 0) continue;
        //printf("ind at: %p, val: %d\n", ind, *ind);
        Vec3 *orig = av_get_type(vxs, *ind, Vec3);
        key[0] = (long)(orig->x * 65536);
        key[1] = (long)(orig->y * 65536);
        key[2] = (long)(orig->z * 65536);
        //printf("orig: (%f, %f, %f), key: (%ld, %ld, %ld)\n", orig->x, orig->y, orig->z, key[0], key[1], key[2]);
       // printf("i: %d, ind: %d, key: %s, orig: (%f, %f, %f)\n", i, *ind, key, orig->x, orig->y, orig->z);
        if((out = hashmapGet(vmap, key)) == NULL) { // unique, we good
            hashmapPut(vmap, key, ind);
        }
        else { // already in there
            Vec3 *outv = av_get_type(vxs, *out, Vec3);
            okey[0] = (long)(outv->x * 65536);
            okey[1] = (long)(outv->y * 65536);
            okey[2] = (long)(outv->z * 65536);
            if(equalsLong3(key, okey))    
                *ind = *out;
        }
    }
}