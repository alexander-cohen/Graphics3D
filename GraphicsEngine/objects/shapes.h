//
// Created by gmarks on 7/6/17.
//

#ifndef GRAPHICS3D_SHAPES_H
#define GRAPHICS3D_SHAPES_H

#include "datastructures/arrayvec.h"
#include <math.h>
#include "linalg/vector.h"
#include "pipeline/primitives.h"
#include "datastructures/hashmap.h"

// arrayvec *ptsTrisToTriangles(arrayvec *pts, arrayvec *tris, int mat);

arrayvec *spherePoints(double x, double y, double z, double radius, int steps);
arrayvec *sphereTris(int steps);
void fix_overlap(arrayvec *vxs, arrayvec *tri_idxs);

#endif //GRAPHICS3D_SHAPES_H