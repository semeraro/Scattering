/*
 * convert a domain to a volume. 
 */
#include "Domain.h"
#include <openvkl/openvkl.h>
#include <iostream>
#include <sstream>
#include "netcdf.hh"
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <ios>

using namespace std;

inline VKLDevice device;

VKLDevice getVKLDevice() { return device; }

VKLVolume DomainToVolume(const Domain &cloud) {
    int nx = cloud.coords.x;
    int ny = cloud.coords.y;
    int nz = cloud.coords.z;
    float spx = cloud.spacing.x;
    float spy = cloud.spacing.y;
    float spz = cloud.spacing.z;
    float orgx = cloud.origin.x;
    float orgy = cloud.origin.y;
    float orgz = cloud.origin.z;
    long count = cloud.npts;
    VKLDevice dev = getVKLDevice();
    if(!dev) {
        std::cout << "bogus device" << std::endl;
    }
    VKLVolume volume = vklNewVolume(dev,"structuredRegular");
    vklSetVec3i(volume,"dimensions",nx,ny,nz);
    vklSetVec3f(volume,"gridOrigin",orgx,orgy,orgz);
    vklSetVec3f(volume,"gridSpacing",spx,spy,spz);
    VKLData data0 = vklNewData(dev,count,VKL_FLOAT,cloud.variable,VKL_DATA_SHARED_BUFFER,0);
    vklSetData(volume,"data",data0);
    vklRelease(data0);
    vklCommit(volume);
    vkl_range1f valuerange = vklGetValueRange(volume,0);
    std::cout << "Min: " << valuerange.lower << " Max: " << valuerange.upper <<std::endl;
    return volume;
}
