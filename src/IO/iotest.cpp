/*
 *
 */
#include "Domain.h"
#ifdef WITH_NETCDF
#include "NetcdfStructuredVolume.h"
#endif
#include <openvkl/openvkl.h>
extern VKLVolume DomainToVolume(const Domain&,VKLDevice device);
VKLDevice init_vkl() {
    vklLoadModule("cpu_device");
    VKLDevice device = vklNewDevice("cpu");
    vklDeviceSetInt(device,"logLevel",VKL_LOG_INFO);
    vklDeviceSetString(device,"logOutput","cout");
    vklCommitDevice(device);
    return device;
}
//
// A little test code to validate the IO bits. 
// Open a particular file and create a Domain object.
//
int main(int argc, char **argv) {
    VKLDevice dev = init_vkl();
    // the file spec
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("float.bov");
    std::string filespec = directory+filename;
    std::string fieldname("qi");
    std::cout << "Opening " + filespec + " " << std::endl;
    Domain Storm(filespec,fieldname);
    VKLVolume StormVolume = DomainToVolume(Storm,dev);
#ifdef WITH_NETCDF
    openvkl::testing::NetcdfStructuredVolume volume(filespec,fieldname);
#endif
    std::cout << "Done" << std::endl;
}   
