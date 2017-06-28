#include "input_assembler.h"

render_context *input_assembler(triangle *ts, size_t ntriangles, matarrayvec materials) {
	render_context *rc = malloc(sizeof(render_context));
    rc->vlist = new_vec3arrayvec();
    rc->nlist = new_vec3arrayvec();
    rc->tlist = new_vec2arrayvec();
    rc->mlist = new_intarrayvec();
    rc->materials = new_matarrayvec();
    int i;
    triangle t;
    for(i = 0; i < ntriangles; i++) {
        t = ts[i];
        rc->vlist->append(rc->vlist, t.p1);
        rc->vlist->append(rc->vlist, t.p2);
        rc->vlist->append(rc->vlist, t.p3);
        rc->nlist->append(rc->nlist, t.n1);
        rc->nlist->append(rc->nlist, t.n2);
        rc->nlist->append(rc->nlist, t.n3);
        rc->tlist->append(rc->tlist, t.t1);
        rc->tlist->append(rc->tlist, t.t2);
        rc->tlist->append(rc->tlist, t.t3);
        rc->mlist->append(rc->mlist, t.mat);
    }
    for(i = 0; i < materials->len; i++) {
        rc->materials->append(rc->materials, materials->data[i]);
    }
    return rc;
}