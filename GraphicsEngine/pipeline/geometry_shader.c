#include "geometry_shader.h"
void geometry_shader(render_context *rc){
    Vec3 *v1, *v2, *v3, *n1, *n2, *n3;
    Vec2 *t1, *t2, *t3;
    int i, i3;
    printf("running geom shader on %d triangles...\n", rc->mlist->used_len);
    for(i3 = 0, i = 0; i < rc->mlist->used_len; i++, i3+=3) {
        //printf("norming tri of pts %d, %d, %d\n", i3, i3+1, i3+2);
        v1 = av_get_type(rc->vlist, i3, Vec3);
        v2 = av_get_type(rc->vlist, i3+1, Vec3);
        v3 = av_get_type(rc->vlist, i3+2, Vec3);
        n1 = av_get_type(rc->nlist, i3, Vec3);
        n2 = av_get_type(rc->nlist, i3+1, Vec3);
        n3 = av_get_type(rc->nlist, i3+2, Vec3);
        t1 = av_get_type(rc->tlist, i3, Vec2);
        t2 = av_get_type(rc->tlist, i3+1, Vec2);
        t3 = av_get_type(rc->tlist, i3+2, Vec2);
        // flat face normal gen
        Vec3 n = vec3cross(vec3isub(*v3, *v1), vec3isub(*v1, *v2));
        *n1 = *n2 = *n3 = vec3idiv(n, vec3norm(n));
        //printf("norm: (%f, %f, %f)\n", n1->x, n1->y, n1->z);
    }
}