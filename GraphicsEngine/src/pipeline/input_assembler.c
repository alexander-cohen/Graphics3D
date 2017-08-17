#include "input_assembler.h"

render_context *input_assembler(arrayvec *tris, arrayvec *materials) {
	render_context *rc = malloc(sizeof(render_context));
    rc->trilist = av_clone(tris);
    rc->materials = av_clone(materials);
    return rc;
}