#ifndef ALLAV_H
#define ALLAV_H

#include "arrayvec/doublearrayvec.h"
#include "arrayvec/intarrayvec.h"
#include "arrayvec/intptrarrayvec.h"
#include "arrayvec/matarrayvec.h"
#include "arrayvec/triarrayvec.h"
#include "arrayvec/vec2arrayvec.h"
#include "arrayvec/vec3arrayvec.h"
#include "arrayvec/voidptrarrayvec.h"

#define append(av, data) (av->append(av, data))
#define pop(av) (av->pop(av))
#define replace(av, i, data) (av->replace(av, i, data))

//covered by primitives.h
// #undef TYPE
// #undef TYPENAME
// #define TYPE Vec3
// #define TYPENAME vec3
// #include "arrayvec.h"
// #undef TYPE
// #undef TYPENAME
// #define TYPE Vec2
// #define TYPENAME vec2
// #include "arrayvec.h"
#endif