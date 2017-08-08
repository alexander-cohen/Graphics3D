#include "fragment_shader.h"
void fragment_shader(raster_context *rac, int (*shader)(Vec3, Vec3, Vec2, environment, material)){
    int x, y, i = 0;
    Vec3 v = {250, 250, 1e30}; // essentially infinitely far is the point
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

double dipow(double base, unsigned int exp) {
    double res = 1;
    while(exp) {
        if(exp & 1)
            res *= base;
        exp>>=1;
        base *= base;
    }
    return res;
} 

int phong_shader(Vec3 pos, Vec3 n, Vec2 t, environment env, material mat) { // example shader, copied from my old python implementation
    Vec3 v = env.v;
    arrayvec lights = env.lights;
    Vec3 PV = vec3sub(v, pos);
    double _pvn = vec3norm(PV);
    vec3idiv(PV, _pvn);
    Vec3 col = {0,0,0};
    int i;
    for(i = 0; i < lights->used_len; i++) {
        light l = av_get_value(lights, i, light);
        Vec3 PL = vec3sub(l.pos, pos);
        double _pln = vec3norm(PL);
        vec3idiv(PL, _pln);
        double Lmn = vec3dot(PL, n);
        Vec3 Rm = vec3sub(vec3mul(n, 2*Lmn), PL);
        double diffuse = Lmn < 0 ? 0 : Lmn;
        double RmV = vec3dot(Rm, PV);
        double spectral = RmV < 0 ? 0 : dipow(RmV, mat.shininess);
        Vec3 light_add = vec3add(vec3add(vec3elmul(mat.Ka, l.Ia), vec3elmul(mat.Kd, vec3mul(l.Id, diffuse))), vec3elmul(mat.Ks, vec3mul(l.Is, spectral)));
        vec3iadd(col, light_add);
    }
    col = vec3cap(col, 0, 1);
    return vec3ToHex(col);
}