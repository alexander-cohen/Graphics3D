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

extern inline Vec2 vec2add(Vec2 v, Vec2 o);
extern inline double vec2cross(Vec2 v, Vec2 o);
extern inline Vec2 vec2div(Vec2 v, double o);
extern inline double vec2dot(Vec2 v, Vec2 o);
extern inline Vec3 vec2ext(Vec2 v, double o);
extern inline Vec2 vec2iadd(Vec2 v, Vec2 o);
extern inline Vec2 vec2idiv(Vec2 v, double o);
extern inline Vec2 vec2imul(Vec2 v, double o);
extern inline Vec2 vec2isub(Vec2 v, Vec2 o);
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
extern inline Vec3 vec3iadd(Vec3 v, Vec3 o);
extern inline Vec3 vec3idiv(Vec3 v, double o);
extern inline Vec3 vec3imul(Vec3 v, double o);
extern inline Vec3 vec3isub(Vec3 v, Vec3 o);
extern inline Vec3 vec3mul(Vec3 v, double o);
extern inline double vec3norm(Vec3 v);
extern inline Vec3 vec3lerp(Vec3 v, Vec3 o, double t);
extern inline Vec3 vec3sub(Vec3 v, Vec3 o);
extern inline Vec4 vec4add(Vec4 v, Vec4 o);
extern inline Vec3 vec4con(Vec4 v);
extern inline Vec4 vec4div(Vec4 v, double o);
extern inline double vec4dot(Vec4 v, Vec4 o);
extern inline Vec4 vec4iadd(Vec4 v, Vec4 o);
extern inline Vec4 vec4idiv(Vec4 v, double o);
extern inline Vec4 vec4imul(Vec4 v, double o);
extern inline Vec4 vec4isub(Vec4 v, Vec4 o);
extern inline Vec4 vec4mul(Vec4 v, double o);
extern inline double vec4norm(Vec4 v);
extern inline Vec4 vec4sub(Vec4 v, Vec4 o);
extern inline Vec4 vec4lerp(Vec4 v, Vec4 o, double t);