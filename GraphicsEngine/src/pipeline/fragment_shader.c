#include "fragment_shader.h"
void process_fragments(raster_context *rac, int (*shader)(Vec3, Vec3, Vec2, environment, material)){
    int x, y, i = 0;
    //Vec3 v = {250, 250, 1e30}; // essentially infinitely far is the point
    for(y = 0; y < rac->height; y++) {
        for(x = 0; x < rac->width; x++, i++) {
            if(rac->mat_buffer[i] > -1) {
                material mat = av_get_value (rac->materials, rac->mat_buffer[i], material);
                Vec3 n = rac->n_buffer[i];
                rac->color_buffer[i] = shader((Vec3){x, y, rac->z_buffer[i]}, n, (Vec2){0,0}, rac->env, mat); // TODO include TCs in rasterization
            }
            else {
                rac->color_buffer[i] = 0xFFFFFF;
            }
        }
    }
}

int default_fragment_shader(Vec3 v, Vec3 n, Vec2 t, environment env, material mat) {
    return vec3ToHex(vec3cap(mat.Ka, 0, 1));
}