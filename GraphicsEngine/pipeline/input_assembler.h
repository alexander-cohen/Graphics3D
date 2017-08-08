#pragma once
#include "renderer.h"
#include "primitives.h"
#include "linalg/vector.h"
#include "datastructures/arrayvec.h"
render_context *input_assembler(arrayvec *tris, arrayvec *materials, arrayvec *lights);