#include "fragment_shader.h"
void fragment_shader(raster_context *rac){
    int x, y, i = 0;
    for(y = 0; y < rac->height; y++) {
        for(x = 0; x < rac->width; x++, i++) {
            if(rac->mat_buffer[i] > -1) {
                rac->color_buffer[i] = rac->materials->data[rac->mat_buffer[i]].color;
            }
        }
    }
}