//
// Created by gmarks on 7/6/17.
//

#include "vnt.h"

void gen_surface_normals(arrayvec *tris){  // tri array vec
    triangle *tri;
    int i;
    printf("running geom shader on %d triangles...\n", tris->used_len);
    for(i = 0; i < tris->used_len; i++) {
        //printf("norming tri of pts %d, %d, %d\n", i3, i3+1, i3+2);
        tri = av_get_type(tris, i, triangle);

        // flat face normal gen
        Vec3 n = vec3cross(vec3isub(tri->p3, tri->p1), vec3isub(tri->p1, tri->p2));
        tri->n1 = tri->n2 = tri->n3 = vec3idiv(n, vec3norm(n));
        //printf("norm: (%f, %f, %f)\n", n1->x, n1->y, n1->z);
    }
}