#include "renderer.h"
#include "input_assembler.h"
#include "vertex_shader.h"
#include "tessellation_shader.h"
#include "geometry_shader.h"
#include "clipper.h"
#include "fragment_shader.h"

int *render(triangle *ts, size_t ntriangles, matarrayvec materials) {
    render_context *rc = input_assembler(ts, ntriangles, materials);
    vertex_shader(rc);
    tessellation_shader(rc);
    geometry_shader(rc);
    clip(rc);
    raster_context *rac = rasterizer(rc);
    fragment_shader(rac);
    free_vec3arrayvec(rc->vlist);
    free_vec3arrayvec(rc->nlist);
    free_vec2arrayvec(rc->tlist);
    free_matarrayvec(rc->materials);
    free_intarrayvec(rc->mlist);
    free(rc);
    free(rac->z_buffer);
    free(rac->mat_buffer);
    int *ret = rac->color_buffer;
    free(rac);
    return ret;
}

