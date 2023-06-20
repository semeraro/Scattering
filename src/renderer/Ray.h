// A ray struct
#pragma once

#include "rkcommon/math/box.h"
#include "rkcommon/math/vec.h"
#include <vector>

using namespace rkcommon::math;

    struct Ray
    {
      vec3f org;
      vec3f dir;
      range1f t;
    };

typedef std::vector<Ray> RayVector;