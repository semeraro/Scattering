/*
 *
 */
#pragma once
#ifdef WITH_NETCDF
#include "netcdf.h"
static const int NC_ERR = 2;
#endif
#include "rkcommon/math/vec.h"
#include <openvkl/openvkl.h>
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
struct BOV_METADATA {
    int nx,ny,nz;
    int bx,by,bz;
    string rawfilename;
    string variable;
    string endian;
    string format;
    bool divide;
    float time;
    void* thedata;
    
};
BOV_METADATA read_bov_metadata(string filename);
struct Domain {
    unsigned long npts;
    vec3i coords;
    vec3f origin;
    vec3f spacing;
    float *variable;
    string fnext;
    Domain(string filename, string fieldname,bool metadataonly = true);
    int LoadData(string filename, string fieldname,bool metadataonly);
};
VKLVolume DomainToVolume(const Domain &cloud, VKLDevice device,VKLVolume oldvolume=nullptr);