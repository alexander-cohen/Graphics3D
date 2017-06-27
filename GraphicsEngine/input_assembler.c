#include "input_assembler.h"
#include "renderer.h"

render_context *input_assembler(triangle *ts, size_t ntriangles, matarrayvec materials) {
	render_context *rc = malloc(sizeof(render_context));
    rc->vlist = new_vec3arrayvec();
    rc->nlist = new_vec3arrayvec();
    rc->tlist = new_vec2arrayvec();
    rc->mlist = new_intarrayvec();
    rc->materials = materials;
    int i;
    for(i = 0; i < ntriangles; i++) {
        rc->vlist->append(rc->vlist, ts[i].p1);
        rc->vlist->append(rc->vlist, ts[i].p2);
        rc->vlist->append(rc->vlist, ts[i].p3);
        rc->nlist->append(rc->nlist, ts[i].n1);
        rc->nlist->append(rc->nlist, ts[i].n2);
        rc->nlist->append(rc->nlist, ts[i].n3);
        rc->tlist->append(rc->tlist, ts[i].t1);
        rc->tlist->append(rc->tlist, ts[i].t2);
        rc->tlist->append(rc->tlist, ts[i].t3);
        rc->mlist->append(rc->mlist, ts[i].mat);
    }
    return rc;
}