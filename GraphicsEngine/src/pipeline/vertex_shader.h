#pragma once
#include "renderer.h"
#include "helper.h"

void process_vertices(render_context *rc, void (*shader)(Vec3 *, Vec3 *, Vec2 *));