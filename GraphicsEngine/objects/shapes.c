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
            av_append_literal(tris, idx, int);
            av_append_literal(tris, iN, int);
            av_append_literal(tris, iT, int);
            av_append_literal(tris, iN, int);
            av_append_literal(tris, iTN, int);
            av_append_literal(tris, iT, int);
        }
    }
    return tris;
}

void fix_overlap(arrayvec *vxs, arrayvec *tri_idxs) {
    map_t vmap = hashmap_new();
    int i, one = 1, zero = 0;
    Vec3 *real;
    int **out;
    char *key = malloc(sizeof(Vec3) + 1);
    key[sizeof(Vec3)] = NULL;
    for(i = 0; i < tri_idxs->used_len; i++) {
        int *ind = av_get_type(tri_idxs, i, int);
        printf("tri idx->data points to %p\n", tri_idxs->data);
        printf("ind at: %p, val: %d\n", ind, *ind);
        Vec3 *orig = av_get_type(vxs, *ind, Vec3);
        memcpy(key, orig, sizeof(Vec3));
        printf("tri idx->data points to %p\n", tri_idxs->data);
        if(hashmap_get(vmap, key, out) == MAP_MISSING) { // unique, we good
            printf("tri idx->data points to %p\n", tri_idxs->data);
            hashmap_put(vmap, key, &ind);
            printf("tri idx->data points to %p\n", tri_idxs->data);
        }
        else { // already in there
            printf("replaced %i with %i\n", *ind, **out);
            av_set(tri_idxs, *out, i, false);
        }
    }
}