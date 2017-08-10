#pragma once
#include "renderer.h"
#include "helper.h"

void process_fragments(raster_context *rac, int (*shader)(Vec3, Vec3, Vec2, environment, material));
int phong_shader(Vec3 pos, Vec3 n, Vec2 t, environment env, material mat);