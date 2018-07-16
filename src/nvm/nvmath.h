#ifndef NV_MATH_H
#define NV_MATH_H

#include <math.h>

#include "nvvector.h"
#include "nvmatrix.h"
#include "nvquaternion.h"

#define NV_PI   float(3.1415926535897932384626433832795)

namespace nv {

typedef VEC2<float> vec2;
typedef VEC2<int>   ivec2;
typedef VEC3<float> vec3;
typedef VEC3<int>   ivec3;
typedef VEC4<float> vec4;
typedef VEC4<int>   ivec4;
typedef matrix4<float> mat4;
typedef quaternion<float> quaternionf;
typedef quaternion<float> quat;
};

using namespace nv;

#endif