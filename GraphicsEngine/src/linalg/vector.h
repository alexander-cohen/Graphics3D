#pragma once

#include "helper.h"
#include <math.h>

typedef struct {
	double x, y;
} Vec2;

typedef struct {
	double x, y, z;
} Vec3;

typedef struct {
	double x, y, z, w;
} Vec4;

#define vec2isub(v, o) do { \
    (v).x -= o.x;\
    (v).y -= o.y;\
} while(0)

#define vec2imul(v, o) do { \
    (v).x *= o;\
    (v).y *= o;\
} while(0)

#define vec2idiv(v, o) do { \
    (v).x /= o;\
    (v).y /= o;\
} while(0)

#define vec3iadd(v, o) do { \
    (v).x += o.x;\
    (v).y += o.y;\
    (v).z += o.z;\
} while(0)

#define vec3isub(v, o) do { \
    (v).x -= o.x;\
    (v).y -= o.y;\
    (v).z -= o.z;\
} while(0)

#define vec3imul(v, o) do { \
    (v).x *= o;\
    (v).y *= o;\
    (v).z *= o;\
} while(0)

#define vec3idiv(v, o) do { \
    (v).x /= o;\
    (v).y /= o;\
    (v).z /= o;\
} while(0)

#define vec4iadd(v, o) do { \
    (v).x += o.x; \
    (v).y += o.y;\
    (v).z += o.z;\
    (v).w += o.w;\
} while(0)

#define vec4isub(v, o) do { \
    (v).x -= o.x;\
    (v).y -= o.y;\
    (v).z -= o.z;\
    (v).w -= o.w;\
} while(0)

#define vec4imul(v, o) do { \
    (v).x *= o;\
    (v).y *= o;\
    (v).z *= o;\
    (v).w *= o;\
} while(0)

#define vec4idiv(v, o) do { \
    (v).x /= o;\
    (v).y /= o;\
    (v).z /= o;\
    (v).w /= o;\
} while(0)



extern inline Vec2 vec2add(Vec2 v, Vec2 o);
extern inline double vec2cross(Vec2 v, Vec2 o);
extern inline Vec2 vec2div(Vec2 v, double o);
extern inline double vec2dot(Vec2 v, Vec2 o);
extern inline Vec3 vec2ext(Vec2 v, double o);
extern inline Vec2 vec2mul(Vec2 v, double o);
extern inline double vec2norm(Vec2 v);
extern inline Vec2 vec2sub(Vec2 v, Vec2 o);
extern inline Vec2 vec2lerp(Vec2 v, Vec2 o, double t);
extern inline Vec3 vec3add(Vec3 v, Vec3 o);
extern inline Vec2 vec3con(Vec3 v);
extern inline Vec3 vec3cross(Vec3 v, Vec3 o);
extern inline Vec3 vec3div(Vec3 v, double o);
extern inline double vec3dot(Vec3 v, Vec3 o);
extern inline Vec4 vec3ext(Vec3 v, double o);
extern inline Vec3 vec3mul(Vec3 v, double o);
extern inline double vec3norm(Vec3 v);
extern inline Vec3 vec3lerp(Vec3 v, Vec3 o, double t);
extern inline Vec3 vec3sub(Vec3 v, Vec3 o);
extern inline Vec4 vec4add(Vec4 v, Vec4 o);
extern inline Vec3 vec4con(Vec4 v);
extern inline Vec4 vec4div(Vec4 v, double o);
extern inline double vec4dot(Vec4 v, Vec4 o);
extern inline Vec4 vec4mul(Vec4 v, double o);
extern inline double vec4norm(Vec4 v);
extern inline Vec4 vec4sub(Vec4 v, Vec4 o);
extern inline Vec4 vec4lerp(Vec4 v, Vec4 o, double t);
extern inline Vec2 vec2elmul(Vec2 v, Vec2 o);
extern inline Vec3 vec3elmul(Vec3 v, Vec3 o);
extern inline Vec4 vec4elmul(Vec4 v, Vec4 o);
extern inline double cap(double f, double lo, double hi);
extern inline Vec2 vec2cap(Vec2 v, double lo, double hi);
extern inline Vec3 vec3cap(Vec3 v, double lo, double hi);
extern inline Vec4 vec4cap(Vec4 v, double lo, double hi);