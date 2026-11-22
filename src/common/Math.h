// partial Copyright (c) 2013 GarageGames, LLC
#pragma once

#include "Types.h"

#undef M_PI
#undef M_SQRT2
#undef M_2PI

#define M_PI         3.14159265358979323846
#define M_SQRT2      1.41421356237309504880

#define M_2PI        (M_PI * 2.0)
#define M_SQRTHALF   0.7071067811865475244008443

#define M_PI_F         3.14159265358979323846f
#define M_SQRT2_F      1.41421356237309504880f

#define M_2PI_F        (M_PI * 2.0f)
#define M_SQRTHALF_F   0.7071067811865475244008443f

#define M_HALFPI_F     1.57079632679489661923f

#define M_CONST_E_F    2.7182818284590452353602874f

#define POINT_EPSILON (0.0001f) ///< Epsilon for point types.

#define M_DEG2RAD   M_PI / 180.f;

namespace OGO {

    inline U32 BIT(U32 x) {return 1 << (x);}
}
