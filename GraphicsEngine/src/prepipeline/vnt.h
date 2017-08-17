//
// Created by gmarks on 7/6/17.
//

#ifndef GRAPHICS3D_VNT_H
#define GRAPHICS3D_VNT_H
#include "datastructures/arrayvec.h"
#include "pipeline/primitives.h"
void gen_surface_normals(arrayvec *tris);
void gen_vertex_normals(arrayvec *,arrayvec *,arrayvec *,arrayvec *);
arrayvec *VTNT_to_AV(arrayvec *,arrayvec *,arrayvec *,arrayvec *,arrayvec *);
#endif //GRAPHICS3D_VNT_H
