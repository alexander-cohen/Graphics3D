#include "input_assembler.h"

render_context *input_assembler(arrayvec tris, size_t ntriangles, arrayvec materials) {
	render_context *rc = malloc(sizeof(render_context));
    rc->vlist = new_arrayvec();
    rc->nlist = new_arrayvec();
    rc->tlist = new_arrayvec();
    rc->mlist = new_arrayvec();
    rc->materials = new_arrayvec();
    int i;
    triangle t;
    for(i = 0; i < ntriangles; i++) {
        t = get(tris, triangle, i);
        append(rc->vlist, Vec3, t.p1);
        printf("p1: %f, %f, %f\n", t.p1.x, t.p1.y, t.p1.z);
        Vec3 vvv = pop(rc->vlist, Vec3);
        printf("just added: %f, %f, %f\n", vvv.x, vvv.y, vvv.z);
        append(rc->vlist, Vec3, t.p1);
        append(rc->vlist, Vec3, t.p2);
        append(rc->vlist, Vec3, t.p3);
        append(rc->nlist, Vec3, t.n1);
        append(rc->nlist, Vec3, t.n2);
        append(rc->nlist, Vec3, t.n3);
        append(rc->tlist, Vec2, t.t1);
        append(rc->tlist, Vec2, t.t2);
        append(rc->tlist, Vec2, t.t3);
        append(rc->mlist, int, t.mat);
    }
    for(i = 0; i < materials->len; i++) {
        append(rc->materials, material, get(materials, material, i));
    }
    return rc;
}