#pragma once
#include "renderer.h"

void geometry_shader(render_context *rc, arrayvec *(*shader)(triangle *));