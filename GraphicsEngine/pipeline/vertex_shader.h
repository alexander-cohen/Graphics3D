#pragma once
#include "renderer.h"
#include "helper.h"

void vertex_shader(render_context *rc, void (*shader)(Vec3, Vec3, Vec2));