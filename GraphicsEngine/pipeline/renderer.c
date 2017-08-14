#include "renderer.h"
#include "input_assembler.h"
#include "vertex_shader.h"
#include "tessellation_shader.h"
#include "geometry_shader.h"
#include "clipper.h"
#include "fragment_shader.h"
#include "rasterizer.h"
#include "postprocess.h"

int *render(arrayvec *tris, arrayvec *materials, environment env, shaderlist shaders) {
    render_context *rc = input_assembler(tris, materials);
    int i;
    for(i = 0; i < shaders.render_shaders->used_len; i++) {
        render_shader *rs = av_get_type(shaders.render_shaders, i, render_shader);
        if(rs->type == VERTEX)
            process_vertices(rc, rs->func);
        else if(rs->type == TESSELLATION)
            process_tessellation(rc, rs->func);
        else if(rs->type == GEOMETRY)
            process_geometry(rc, rs->func);
        else {
            printf("Bad render shader type #%d\n", rs->type);
            exit(1);
        }
    }
    clip(rc, 512, 512); // might not be necessary because our triangle algorithm uses bbox
    raster_context *rac = rasterizer(rc, env, 512, 512);
    process_fragments(rac, shaders.fragment_shader);
    av_clear(rc->trilist);
    av_clear(rc->materials);
    free(rc->trilist);
    free(rc->materials);
    free(rc);
    printf("zbuf corners %f %f %f %f", rac->z_buffer[0], rac->z_buffer[511], rac->z_buffer[511*512], rac->z_buffer[512*512 - 1]);
    free(rac->z_buffer);
    free(rac->mat_buffer);
    free(rac->n_buffer);
    int *ret = rac->color_buffer;
    for(i = 0; i < shaders.postprocess_shaders->used_len; i++) {
        postprocess_shader *pps = av_get_type(shaders.postprocess_shaders, i, postprocess_shader);
        if(pps->type == SINGLE)
            ret = postprocess(ret, 512, 512, pps->func);
        else if(pps->type == ALL)
            ret = ((int *(*)(int *, int, int))pps->func)(ret, 512, 512);
        else {
            printf("Bad postprocess shader type #%d\n", pps->type);
        }
    }
    if(ret != rac->color_buffer)
        free(rac->color_buffer);
    printf("%d %d", rac->width, rac->height);
    free(rac);
    printf("first col in the buf: %d", ret[0]);
    return ret;
}

