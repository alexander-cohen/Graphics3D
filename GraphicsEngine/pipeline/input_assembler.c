#include "input_assembler.h"

render_context *input_assembler(arrayvec *tris, arrayvec *materials) {
	render_context *rc = malloc(sizeof(render_context));
    rc->vlist = av_create (0, sizeof (Vec3));
    rc->nlist = av_create (0, sizeof (Vec3));
    rc->tlist = av_create (0, sizeof (Vec2));
    rc->mlist = av_create (0, sizeof (int));
    rc->materials = av_create (0, sizeof (material));
    int i;
    triangle t;
    for(i = 0; i < tris->used_len; i++) {
        t = av_get_value(tris, i, triangle);
        av_append(rc->vlist, &t.p1, false);
        printf("p1: %f, %f, %f\n", t.p1.x, t.p1.y, t.p1.z);
        Vec3 vvv;
        av_pop_value (rc->vlist, &vvv, Vec3);
        printf("just added: %f, %f, %f\n", vvv.x, vvv.y, vvv.z);
        av_append(rc->vlist, &t.p1, false);
        av_append(rc->vlist, &t.p2, false);
        av_append(rc->vlist, &t.p3, false);
        av_append(rc->nlist, &t.n1, false);
        av_append(rc->nlist, &t.n2, false);
        av_append(rc->nlist, &t.n3, false);
        av_append(rc->tlist, &t.t1, false);
        av_append(rc->tlist, &t.t2, false);
        av_append(rc->tlist, &t.t3, false);
        av_append(rc->mlist, &t.mat, false);
    }
    for(i = 0; i < materials->used_len; i++) {
        av_append(rc->materials, av_get_type(materials, i, material), false);
    }
    return rc;
}