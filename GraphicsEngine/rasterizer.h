//
// Created by gmarks on 6/26/17.
//

#ifndef GRAPHICS3D_RASTERIZER_H
#define GRAPHICS3D_RASTERIZER_H

#include "renderer.h"
#include "input_assembler.h"
#include "graphics2d/graphics2d.h"
#include "linalg/vector.h"

raster_context *rasterizer(render_context *rc, int width, int height);
void raster_tri(raster_context *rac, triangle tri);

#endif //GRAPHICS3D_RASTERIZER_H
