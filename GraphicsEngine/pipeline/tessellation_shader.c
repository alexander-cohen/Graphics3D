#include "tessellation_shader.h"

void tessellation_shader(render_context *rc, arrayvec *(*shader)(triangle *)){
	int i;
	triangle *tri;
	arrayvec *newtris;
	for(i = 0; i < rc->trilist->used_len; i++) {
		tri = av_get_type(rc->trilist, i, triangle);
		newtris = shader(tri);
		if(newtris->used_len == 0) {
			if(i != rc->trilist->used_len-1) {
				av_set(rc->trilist, av_get(rc->trilist, rc->trilist->used_len-1), i, false);
			}
			av_pop(rc->trilist);
		}
		else if(newtris->used_len == 1) {
			av_set(rc->trilist, av_get(newtris, 0), i, false);
		}
		else {
			av_set(rc->trilist, av_get(newtris, 0), i, false);
			bool reall = false;
			while(vec->reserved_len < vec->used_len + newtris->used_len - 1) {
				vec->reserved_len *= 2;
				reall = true;
			}
			if(reall)
				vec->data = realloc (vec->data, vec->elem_size * (vec->reserved_len));
			memcpy(vec->data + vec->elem_size * vec->used_len, newtris->data + newtris->elem_size, vec->elem_size * newtris->used_len);
			vec->used_len += newtris->used_len;
		}
	}
}