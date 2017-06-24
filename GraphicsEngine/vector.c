#include "vector.h"


// basic arithmetic
Vec2 vec2iadd(Vec2 v, Vec2 o) {
    v.x+=o.x;
    v.y+=o.y;
    return v;
}

Vec2 vec2add(Vec2 v, Vec2 o) {
    return (Vec2){v.x+o.x,v.y+o.y};
}

Vec2 vec2isub(Vec2 v, Vec2 o) {
    v.x-=o.x;
    v.y-=o.y;
    return v;
}

Vec2 vec2sub(Vec2 v, Vec2 o) {
    return (Vec2){v.x-o.x,v.y-o.y};
}

Vec2 vec2imul(Vec2 v, double o) {
    v.x*=o;
    v.y*=o;
    return v;
}

Vec2 vec2mul(Vec2 v, double o) {
    return (Vec2){v.x*o,v.y*o};
}

Vec2 vec2idiv(Vec2 v, double o) {
    v.x/=o;
    v.y/=o;
    return v;
}

Vec2 vec2div(Vec2 v, double o) {
    return (Vec2){v.x/o,v.y/o};
}

Vec3 vec3iadd(Vec3 v, Vec3 o) {
    v.x+=o.x;
    v.y+=o.y;
    v.z+=o.z;
    return v;
}

Vec3 vec3add(Vec3 v, Vec3 o) {
    return (Vec3){v.x+o.x,v.y+o.y,v.z+o.z};
}

Vec3 vec3isub(Vec3 v, Vec3 o) {
    v.x-=o.x;
    v.y-=o.y;
    v.z-=o.z;
    return v;
}

Vec3 vec3sub(Vec3 v, Vec3 o) {
    return (Vec3){v.x-o.x,v.y-o.y,v.z-o.z};
}

Vec3 vec3imul(Vec3 v, double o) {
    v.x*=o;
    v.y*=o;
    v.z*=o;
    return v;
}

Vec3 vec3mul(Vec3 v, double o) {
    return (Vec3){v.x*o,v.y*o,v.z*o};
}

Vec3 vec3idiv(Vec3 v, double o) {
    v.x/=o;
    v.y/=o;
    v.z/=o;
    return v;
}

Vec3 vec3div(Vec3 v, double o) {
    return (Vec3){v.x/o,v.y/o,v.z/o};
}

Vec4 vec4iadd(Vec4 v, Vec4 o) {
    v.x+=o.x;
    v.y+=o.y;
    v.z+=o.z;
    v.w+=o.w;
    return v;
}

Vec4 vec4add(Vec4 v, Vec4 o) {
    return (Vec4){v.x+o.x,v.y+o.y,v.z+o.z,v.w+o.w};
}

Vec4 vec4isub(Vec4 v, Vec4 o) {
    v.x-=o.x;
    v.y-=o.y;
    v.z-=o.z;
    v.w-=o.w;
    return v;
}

Vec4 vec4sub(Vec4 v, Vec4 o) {
    return (Vec4){v.x-o.x,v.y-o.y,v.z-o.z,v.w-o.w};
}

Vec4 vec4imul(Vec4 v, double o) {
    v.x*=o;
    v.y*=o;
    v.z*=o;
    v.w*=o;
    return v;
}

Vec4 vec4mul(Vec4 v, double o) {
    return (Vec4){v.x*o,v.y*o,v.z*o,v.w*o};
}

Vec4 vec4idiv(Vec4 v, double o) {
    v.x/=o;
    v.y/=o;
    v.z/=o;
    v.w/=o;
    return v;
}

Vec4 vec4div(Vec4 v, double o) {
    return (Vec4){v.x/o,v.y/o,v.z/o,v.w/o};
}