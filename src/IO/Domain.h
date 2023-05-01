/*
 *
 */
#pragma once
#ifdef WITH_NETCDF
#include "netcdf.h"
static const int NC_ERR = 2;
#endif
#include "rkcommon/math/vec.h"
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <string>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <ios>
using namespace std;
using namespace rkcommon::math;
struct Domain {
    long npts;
    vec3i coords;
    vec3f origin;
    vec3f spacing;
    float *variable;
    string fnext;
    Domain(string filename, string fieldname);
    int LoadData(string filename, string fieldname);
};
