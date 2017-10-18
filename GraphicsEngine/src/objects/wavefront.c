// Used for decoding wavefront (.obj) object files.
#include "wavefront.h"

typedef struct {
	Vec3 *v;
	Vec3 *n;
	Vec2 *t;
} wf_vnt;

char *read_line(FILE *file) {
	int line_len = 256;
	int cur_len = 0;
	char ch = -10;
	char *line = malloc(line_len);
	while((ch != '\n') && (ch != EOF)) {
		if(cur_len >= line_len) {
			line_len *= 2;
			line = realloc(line, line_len);
		}
		ch = getc(file);
		line[cur_len] = ch;
		cur_len++;
	}
	if(cur_len <= 1 && line[0] == EOF) { // instant EOF
		free(line);
		return NULL;
	}
	if(cur_len >= line_len) {
		line = realloc(line, line_len + 1);
		line[line_len] = 0;
	}
	return line;
}

void parse_vertex(char *line, arrayvec *vxs) {
	while(*line != 'v')
		line++;
	line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double x = atof(line);
	while(*line != ' ' && *line != '\t')
		line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double y = atof(line);
	while(*line != ' ' && *line != '\t')
		line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double z = atof(line);
	av_append(vxs, &(Vec3){x, y, z}, 0);
}

void parse_texcor(char *line, arrayvec *tex_cors) {
	while(*line != 't')
		line++;
	line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double u = atof(line);
	while(*line != ' ' && *line != '\t')
		line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double v = atof(line);
	av_append(tex_cors, &(Vec2){u, v}, 0);
}

void parse_norm(char *line, arrayvec *norms) {
	while(*line != 'n')
		line++;
	line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double x = atof(line);
	while(*line != ' ' && *line != '\t')
		line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double y = atof(line);
	while(*line != ' ' && *line != '\t')
		line++;
	while(*line == ' ' || *line == '\t')
		line++;
	double z = atof(line);
	av_append(norms, &(Vec3){x, y, z}, 0);
}

void parse_face(char *line, arrayvec *tri_cors, arrayvec *vxs, arrayvec *norms, arrayvec *tex_cors, arrayvec *extra_vnt) {
	while(*line != 'f')
		line++;
	line++;
	int i = 0;
	for(; i < 3; i++) {
		while(*line == ' ' || *line == '\t')
			line++;
		int idx = atoi(line) - 1;
		while(*line != ' ' && *line != '\t' && *line != '/' && *line != '\n')
			line++;
		if(*line == '/') {
			int v_idx = idx;
			int t_idx = -1;
			int n_idx = -1;
			line++;
			if(*line == '/') {
				line++;
				n_idx = atoi(line) - 1;
			}
			else {
				t_idx = atoi(line) - 1;
				while(*line != ' ' && *line != '\t' && *line != '/')
					line++;
				if(*line == '/') {
					line++;
					n_idx = atoi(line) - 1;
				}
			}
			while(*line != ' ' && *line != '\t')
				line++;
			if(v_idx == n_idx && n_idx == t_idx)
				av_append(tri_cors, &idx, 0);
			else {
				Vec3 *n, *v = av_get(vxs, v_idx);
				Vec2 *t;
				if(t_idx == -1)
					t = &(Vec2){0, 0};
				else
					t = av_get(tex_cors, t_idx);
				if(n_idx == -1)
					n = &(Vec3){1, 0, 0};
				else
					n = av_get(norms, n_idx);
				int vnt_idx = -extra_vnt->used_len - 1;
				av_append(extra_vnt, &(wf_vnt){v, n, t}, 0);
				av_append(tri_cors, &vnt_idx, 0);
			}
		}
		else {
			av_append(tri_cors, &idx, 0);
			printf("adding idx %d\n", idx);
		}
	}
}

#define OBJ_REGEX(n) "^[[:space:]]*" n

arrayvec **obj_get_lists_vtnt(char *obj_file) {
	
	arrayvec *vxs = av_create(16, sizeof(Vec3));
	arrayvec *tri_cors = av_create(48, sizeof(int));
	arrayvec *norms = av_create(16, sizeof(Vec3));
	arrayvec *tex_cors = av_create(16, sizeof(Vec2));
	arrayvec *extra_vnt = av_create(8, sizeof(wf_vnt));
	FILE *file = fopen(obj_file, "r");
	if(file == NULL) {
		fprintf(stderr, "File \"%s\" does not exist or is not open for reading.\n", obj_file);
		exit(1);
	}
	char *line;
	regex_t reg_comment, reg_vertex, reg_texcor, reg_norm, reg_face;
	if(regcomp(&reg_comment, OBJ_REGEX("#"), 0)) exit(1);
	if(regcomp(&reg_vertex, OBJ_REGEX("v[[:space:]]"), 0)) exit(2);
	if(regcomp(&reg_texcor, OBJ_REGEX("vt[[:space:]]"), 0)) exit(3);
	if(regcomp(&reg_norm, OBJ_REGEX("vn[[:space:]]"), 0)) exit(4);
	if(regcomp(&reg_face, OBJ_REGEX("f[[:space:]]"), 0)) exit(5);
	while((line = read_line(file)) != NULL) {
		if(!regexec(&reg_vertex, line, 0, NULL, 0))
			parse_vertex(line, vxs);
		else if(!regexec(&reg_texcor, line, 0, NULL, 0))
			parse_texcor(line, tex_cors);
		else if(!regexec(&reg_norm, line, 0, NULL, 0))
			parse_norm(line, norms);
		else if(!regexec(&reg_face, line, 0, NULL, 0))
			parse_face(line, tri_cors, vxs, norms, tex_cors, extra_vnt);
		else if(regexec(&reg_comment, line, 0, NULL, 0))
			printf("Warning: line `%s` not understood.\n", line);
	}

	if(extra_vnt->used_len) {
		Vec3 d_n = {1, 0, 0};
		Vec3 d_v = {0, 0, 0};
		Vec2 d_t = {0, 0};
		int biglen = max(max(vxs->used_len, norms->used_len), tex_cors->used_len);
		int i;
		for(i = vxs->used_len; i < biglen; i++) {
			av_append(vxs, &d_v, 0);
		}
		for(i = norms->used_len; i < biglen; i++) {
			av_append(norms, &d_n, 0);
		}
		for(i = tex_cors->used_len; i < biglen; i++) {
			av_append(tex_cors, &d_t, 0);
		}
		for(i = 0; i < tri_cors->used_len; i++) {
			int cor = av_get_value(tri_cors, i, int);
			if(cor < 0) {
				cor = -(cor + 1);
				wf_vnt *vnt = av_get(extra_vnt, cor);
				cor = vxs->used_len;
				av_append(vxs, vnt->v, 0);
				av_append(norms, vnt->n, 0);
				av_append(tex_cors, vnt->t, 0);
				av_set(tri_cors, &cor, i, 0);
			}
		}
	}
	arrayvec **vtnt = malloc(sizeof(arrayvec *) * 4);
	vtnt[0] = vxs;
	vtnt[1] = tri_cors;
	vtnt[2] = norms;
	vtnt[3] = tex_cors;
	return vtnt;
}

#undef OBJ_REGEX
#undef RGX_NUM 
#undef RGX_PLUSINT 
#undef FACE_ELEMENT