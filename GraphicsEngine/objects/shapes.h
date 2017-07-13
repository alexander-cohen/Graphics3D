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
arrayvec *boxPoints(double x, double y, double z, double width, double height, double depth);
arrayvec *boxTris();
arrayvec *torusPoints(double x, double y, double z, double outerRadius, double innerRadius, int outerSteps, int innerSteps);
arrayvec *torusTris(int outerSteps, int innerSteps);
void fix_overlap(arrayvec *vxs, arrayvec *tri_idxs);
void check_orient(arrayvec *vxs, arrayvec *norms);
#endif //GRAPHICS3D_SHAPES_H
