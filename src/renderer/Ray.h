// A ray struct
#pragma once

#include "rkcommon/math/box.h"
#include "rkcommon/math/vec.h"

using namespace rkcommon::math;

    struct Ray
    {
      vec3f org;
      vec3f dir;
      range1f t;
    };