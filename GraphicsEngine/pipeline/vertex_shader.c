#include "vertex_shader.h"

void process_vertices(render_context *rc, void (*shader)(Vec3, Vec3, Vec2)){
	int i;
	triangle *tri;
	for(i = 0; i < rc->trilist->used_len; i++) {
		tri = av_get_type(rc->trilist, i, triangle);
		shader(&(tri->p1), &(tri->n1), &(tri->t1));
		shader(&(tri->p2), &(tri->n2), &(tri->t2));
		shader(&(tri->p3), &(tri->n3), &(tri->t3));
	}
}