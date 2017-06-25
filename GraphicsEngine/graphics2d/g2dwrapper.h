#ifndef G2DWRAPPER_H
#define G2DWRAPPER_H

#include "helper.h"
#include "graphics2d.h"

typedef struct
{
	int type;
	int data[15];
} g2d_primitive;

enum primtive_types {LINE, RECT_FILL, RECT_OUTLINE, CIRCLE_FILL, CIRCLE_OUTLINE};

#endif