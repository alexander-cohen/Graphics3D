#include "renderer.h"
#include "input_assembler.h"
#include "vertex_shader.h"
#include "tessellation_shader.h"
#include "geometry_shader.h"
#include "clipper.h"
#include "fragment_shader.h"

int *render(arrayvec tris, size_t ntriangles, arrayvec materials) {
    render_context *rc = input_assembler(tris, ntriangles, materials);
    vertex_shader(rc);
    tessellation_shader(rc);
    geometry_shader(rc);
    clip(rc, 512, 512); // might not be necessary because our triangle algorithm uses bbox
    raster_context *rac = rasterizer(rc, 512, 512);
    fragment_shader(rac);
    free_arrayvec(rc->vlist);
    free_arrayvec(rc->nlist);
    free_arrayvec(rc->tlist);
    free_arrayvec(rc->materials);
    free_arrayvec(rc->mlist);
    printf("rc mlist %d %d", get(rc->mlist, int, 0), get(rc->mlist, int, 1));
    free(rc);
    printf("zbuf corners %f %f %f %f", rac->z_buffer[0], rac->z_buffer[511], rac->z_buffer[511*512], rac->z_buffer[512*512 - 1]);
    free(rac->z_buffer);
    free(rac->mat_buffer);
    int *ret = rac->color_buffer;
    printf("%d %d", rac->width, rac->height);
    free(rac);
    return ret;
}

