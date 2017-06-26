#include "primitives.h"
#include "arrayvec/tricorarrayvec.h"

typedef struct {
	vec3arrayvec vlist;
	vec3arrayvec nlist;
	vec2arrayvec tlist;
	intarrayvec  mlist;
	matarrayvec  materials;
} render_context;