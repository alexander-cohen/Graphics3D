#pragma once
#include "renderer.h"
#include "helper.h"

void fragment_shader(raster_context *rac);
int phong_shader(Vec3 pos, Vec3 n, arrayvec *lights, Vec3 v, material mat);