/*
 *
 */
#pragma once
#include "netcdf.hh"
#include <vector>
#include <string>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <math.h>
#include "rkcommon/math/vec.h"

using namespace std;
using namespace rkcommon::math;

static const int NC_ERR = 2;
struct Domain {
    long npts;
    vec3i coords;
    vec3f origin;
    vec3f spacing;
    float *variable;
    Domain(string filename, string fieldname);
};
