#include "fragment_shader.h"
void fragment_shader(raster_context *rac){
    int x, y, i = 0;
    for(y = 0; y < rac->height; y++) {
        for(x = 0; x < rac->width; x++, i++) {
            if(rac->mat_buffer[i] > -1) {
                //rac->color_buffer[i] = av_get_value (rac->materials, rac->mat_buffer[i], material).color+(((int)rac->z_buffer[i])<<8);
                Vec3 n = rac->n_buffer[i];
                //printf("n: (%f, %f, %f)\nn scaled: (%d %d %d)\n", n.x, n.y, n.z, (int)(n.x * 127.5 + 127.5), (int)(n.y * 127.5 + 127.5), (int)(n.z * 127.5 + 127.5));
                rac->color_buffer[i] = rgbToHex((int)(n.x * 127.5 + 127.5), (int)(n.y * 127.5 + 127.5), (int)(n.z * 127.5 + 127.5));
                //printf("colbuf: %d\n", rac->color_buffer[i]);
                //printf ("color: %d\n", ;
            }
        }
    }
}