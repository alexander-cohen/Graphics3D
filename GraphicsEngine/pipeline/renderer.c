#include "renderer.h"
#include "input_assembler.h"
#include "vertex_shader.h"
#include "tessellation_shader.h"
#include "geometry_shader.h"
#include "clipper.h"
#include "fragment_shader.h"
#include "rasterizer.h"

int *render(arrayvec *tris, arrayvec *materials, environment env, shaderset shaders) {
    render_context *rc = input_assembler(tris, materials);
    process_vertices(rc, shaders.vertex_shader);
    process_tesselation(rc, shaders.tessellation_shader);
    process_geometry(rc, shaders.geometry_shader);
    clip(rc, 512, 512); // might not be necessary because our triangle algorithm uses bbox
    raster_context *rac = rasterizer(rc, env, 512, 512);
    process_fragments(rac, shaders.fragment_shader);
    av_clear(rc->vlist);
    av_clear(rc->nlist);
    av_clear(rc->tlist);
    av_clear(rc->materials);
    printf("rc mlist %d %d", av_get_value(rc->mlist, 0, int), av_get_value(rc->mlist, 1, int));
    av_clear(rc->mlist);
    free(rc);
    printf("zbuf corners %f %f %f %f", rac->z_buffer[0], rac->z_buffer[511], rac->z_buffer[511*512], rac->z_buffer[512*512 - 1]);
    free(rac->z_buffer);
    free(rac->mat_buffer);
    int *ret = postprocess(rac->color_buffer, shaders.postprocess_shader);
    if(ret != rac->color_buffer)
        free(rac->color_buffer);
    printf("%d %d", rac->width, rac->height);
    free(rac);
    printf("first col in the buf: %d", ret[0]);
    return ret;
}

