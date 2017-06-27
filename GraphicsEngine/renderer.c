#include "renderer.h"
#include "input_assembler.h"
#include "vertex_shader.h"
#include "tessellation_shader.h"
#include "geometry_shader.h"
#include "clipper.h"
#include "fragment_shader.h"

void render(triangle *ts, size_t ntriangles, matarrayvec materials) {
    render_context *rc = input_assembler(ts, ntriangles, materials);
    vertex_shader(rc);
    tessellation_shader(rc);
    geometry_shader(rc);
    clip(rc);
    // raster_context *rac = rasterizer(rc);
    // fragment_shader(rac);
    // output_merger(rac);
    // next_frame(rac->color_buffer);
}