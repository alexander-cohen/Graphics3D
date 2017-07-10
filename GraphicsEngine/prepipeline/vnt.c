//
// Created by gmarks on 7/6/17.
//

#include "vnt.h"

void gen_surface_normals(arrayvec *tris){  // tri array vec
    triangle *tri;
    int i;
    for(i = 0; i < tris->used_len; i++) {
        //printf("norming tri of pts %d, %d, %d\n", i3, i3+1, i3+2);
        tri = av_get_type(tris, i, triangle);

        // flat face normal gen
        Vec3 n = vec3cross(vec3sub(tri->p3, tri->p1), vec3sub(tri->p1, tri->p2));
        tri->n1 = tri->n2 = tri->n3 = vec3div(n, vec3norm(n));
        //printf("norm: (%f, %f, %f)\n", n1->x, n1->y, n1->z);
    }
}

void gen_vertex_normals(arrayvec *vxs, arrayvec *tri_idxs, arrayvec *norms, arrayvec *tcs) { // assumes has VT
    printf("start of gvn tcs->used_len: %d\n", tcs->used_len);
    int i, i1, i2, i3;
    norms->used_len = 0;
    Vec3 zeros = {0,0,0}, p1, p2, p3, *n1, *n2, *n3;

    for(i = 0; i < vxs->used_len; i++) {
        av_append(norms, &zeros, false);
    }
    for(i = 0; i < tri_idxs->used_len; i+=3) { 
        i1 = av_get_value(tri_idxs, i, int);
        i2 = av_get_value(tri_idxs, i+1, int);
        i3 = av_get_value(tri_idxs, i+2, int);

        printf("i1: %d, i2: %d, i3: %d\n", i1, i2, i3);
        p1 = av_get_value(vxs, i1, Vec3);
        p2 = av_get_value(vxs, i2, Vec3);
        p3 = av_get_value(vxs, i3, Vec3);

        n1 = av_get_type(norms, i1, Vec3);
        n2 = av_get_type(norms, i2, Vec3);
        n3 = av_get_type(norms, i3, Vec3);

        Vec3 sn = vec3cross(vec3sub(p3, p1), vec3sub(p1, p2));

        vec3iadd(*n1, sn);
        vec3iadd(*n2, sn);
        vec3iadd(*n3, sn);
        printf("mid of gvn tcs->used_len: %d\n", tcs->used_len);
    }
    for(i = 0; i < norms->used_len; i++) {
        n1 = av_get_type(norms, i, Vec3);
        double normal = vec3norm(*n1);
        vec3idiv(*n1, normal);
    }
    printf("end of gvn tcs->used_len: %d\n", tcs->used_len);
}

arrayvec *VTNT_to_AV(arrayvec *vxs, arrayvec *tri_idxs, arrayvec *norms, arrayvec *tcs) { // assumes has VTNT
    arrayvec *tris = av_create(tri_idxs->used_len/3, sizeof(triangle));
    int i, i1, i2, i3;
    triangle tri;
    for(i = 0; i < tri_idxs->used_len; i+=3) { // if tri_idxs is malformed (i.e. length not div by 3) this will fail, which is good
        i1 = av_get_value(tri_idxs, i, int);
        i2 = av_get_value(tri_idxs, i+1, int);
        i3 = av_get_value(tri_idxs, i+2, int);

        tri.p1 = av_get_value(vxs, i1, Vec3);
        tri.n1 = av_get_value(norms, i1, Vec3);
        tri.t1 = av_get_value(tcs, i1, Vec2);

        tri.p2 = av_get_value(vxs, i2, Vec3);
        tri.n2 = av_get_value(norms, i2, Vec3);
        tri.t2 = av_get_value(tcs, i2, Vec2);

        tri.p3 = av_get_value(vxs, i3, Vec3);
        tri.n3 = av_get_value(norms, i3, Vec3);
        tri.t3 = av_get_value(tcs, i3, Vec2);

        av_append(tris, &tri, false);
    }
    return tris;
}