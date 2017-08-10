#include "geometry_shader.h"
void process_geometry(render_context *rc, arrayvec *(*shader)(triangle)){
	int i;
	triangle tri;
	arrayvec *newtris;
	arrayvec *vec = rc->trilist;
	int orig_len = vec->used_len;
	for(i = 0; i < orig_len-1; i++) {
		printf("geom %d/%d\n", i, orig_len);
		tri = av_get_value(vec, i, triangle);
		newtris = shader(tri);
		if(newtris->used_len == 0) {
			if(i != vec->used_len-1) {
				av_set(vec, av_get(vec, orig_len-1), i, false);
			}
			av_pop(vec);
			orig_len--;
			i--;
		}
		else if(newtris->used_len == 1) {
			av_set(vec, av_get(newtris, 0), i, false);
		}
		else {
			av_set(vec, av_get(newtris, 0), i, false);
			bool reall = false;
			while(vec->reserved_len < vec->used_len + newtris->used_len) {
				vec->reserved_len *= 2;
				reall = true;
			}
			if(reall) {
				vec->data = realloc (vec->data, vec->elem_size * (vec->reserved_len));
				printf("reallocating vec to %d", vec->elem_size * vec->reserved_len);
			}
			memcpy(vec->data + vec->elem_size * vec->used_len, newtris->data + newtris->elem_size, vec->elem_size * newtris->used_len);
			vec->used_len += newtris->used_len;
		}
		free(newtris->data);
		free(newtris);
	}
}

arrayvec *default_geometry_shader(triangle *tri) {
	arrayvec *av = av_create(1, sizeof(triangle));
	av_append(av, tri, false);
	return av;
}	