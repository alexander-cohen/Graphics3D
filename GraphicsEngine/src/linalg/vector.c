#include "vector.h"
#include <math.h>


// basic arithmetic
inline Vec2 vec2sub(Vec2 v, Vec2 o) {
    v.x -= o.x;
    v.y -= o.y;
    return v;
}

inline Vec2 vec2mul(Vec2 v, double o) {
    v.x *= o;
    v.y *= o;
    return v;
}

inline Vec2 vec2div(Vec2 v, double o) {
    v.x /= o;
    v.y /= o;
    return v;
}

inline Vec3 vec3add(Vec3 v, Vec3 o) {
    v.x += o.x;
    v.y += o.y;
    v.z += o.z;
    return v;
}

inline Vec3 vec3sub(Vec3 v, Vec3 o) {
    v.x -= o.x;
    v.y -= o.y;
    v.z -= o.z;
    return v;
}

inline Vec3 vec3mul(Vec3 v, double o) {
    v.x *= o;
    v.y *= o;
    v.z *= o;
    return v;
}

inline Vec3 vec3div(Vec3 v, double o) {
    v.x /= o;
    v.y /= o;
    v.z /= o;
    return v;
}

inline Vec4 vec4add(Vec4 v, Vec4 o) {
    v.x += o.x; 
    v.y += o.y;
    v.z += o.z;
    v.w += o.w;
    return v;
}

inline Vec4 vec4sub(Vec4 v, Vec4 o) {
    v.x -= o.x;
    v.y -= o.y;
    v.z -= o.z;
    v.w -= o.w;
    return v;
}

inline Vec4 vec4mul(Vec4 v, double o) {
    v.x *= o;
    v.y *= o;
    v.z *= o;
    v.w *= o;
    return v;
}

inline Vec4 vec4div(Vec4 v, double o) {
    v.x /= o;
    v.y /= o;
    v.z /= o;
    v.w /= o;
    return v;
}

// other operations

inline double vec2dot(Vec2 v, Vec2 o) {
	return v.x*o.x + v.y*o.y;
}

// not really cross
inline double vec2cross(Vec2 v, Vec2 o) {
	return v.x*o.y - v.y*o.x;
}

inline double vec2norm(Vec2 v) {
	return sqrt(vec2dot(v, v));
}

inline Vec3 vec2ext(Vec2 v, double o) {
	return (Vec3){v.x, v.y, o};
}

inline double vec3dot(Vec3 v, Vec3 o) {
	return v.x*o.x + v.y*o.y + v.z*o.z;
}

// really cross
inline Vec3 vec3cross(Vec3 v, Vec3 o) {
	return (Vec3){v.y*o.z-v.z*o.y, v.z*o.x - v.x*o.z, v.x*o.y-v.y*o.x};
}

inline double vec3norm(Vec3 v) {
	return sqrt (vec3dot(v, v));
}

inline Vec2 vec3con(Vec3 v) {
	return (Vec2){v.x, v.y};
}

inline Vec4 vec3ext(Vec3 v, double o) {
	return (Vec4){v.x, v.y, v.z, o};
}

inline double vec4dot(Vec4 v, Vec4 o) {
	return v.x*o.x + v.y*o.y + v.z*o.z + v.w*o.w;
}

inline double vec4norm(Vec4 v) {
	return sqrt(vec4dot(v, v));
}

inline Vec3 vec4con(Vec4 v) {
	return (Vec3){v.x, v.y, v.z};
}

inline Vec2 vec2lerp(Vec2 v, Vec2 o, double t) {
    return (Vec2){v.x*(1-t) + o.x*t, v.y*(1-t) + o.y*t};
}

inline Vec3 vec3lerp(Vec3 v, Vec3 o, double t) {
    return (Vec3){v.x*(1-t) + o.x*t, v.y*(1-t) + o.y*t, v.z*(1-t) + o.z*t};
}

inline Vec4 vec4lerp(Vec4 v, Vec4 o, double t) {
    return (Vec4){v.x*(1-t) + o.x*t, v.y*(1-t) + o.y*t, v.z*(1-t) + o.z*t, v.w*(1-t) + o.w*t};
}

inline Vec2 vec2elmul(Vec2 v, Vec2 o) {
    return (Vec2){v.x*o.x, v.y*o.y};
}

inline Vec3 vec3elmul(Vec3 v, Vec3 o) {
    return (Vec3){v.x*o.x, v.y*o.y, v.z*o.z};
}

inline Vec4 vec4elmul(Vec4 v, Vec4 o) {
    return (Vec4){v.x*o.x, v.y*o.y, v.z*o.z, v.w*o.w};
}

inline double cap(double f, double lo, double hi) {
    return min(hi, max(f, lo));
}

inline Vec2 vec2cap(Vec2 v, double lo, double hi) {
    return (Vec2){cap(v.x, lo, hi), cap(v.y, lo, hi)};
}

inline Vec3 vec3cap(Vec3 v, double lo, double hi) {
    return (Vec3){cap(v.x, lo, hi), cap(v.y, lo, hi), cap(v.z, lo, hi)};
}

inline Vec4 vec4cap(Vec4 v, double lo, double hi) {
    return (Vec4){cap(v.x, lo, hi), cap(v.y, lo, hi), cap(v.z, lo, hi), cap(v.w, lo, hi)};
}