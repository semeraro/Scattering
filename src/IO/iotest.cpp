/*
 *
 */
#include "Domain.h"
#ifdef WITH_NETCDF
#include "NetcdfStructuredVolume.h"
#endif
#include <openvkl/openvkl.h>
inline VKLDevice device;
extern VKLVolume DomainToVolume(const Domain&);
void init_vkl() {
    vklLoadModule("cpu_device");
    device = vklNewDevice("cpu");
    vklDeviceSetInt(device,"logLevel",VKL_LOG_INFO);
    vklDeviceSetString(device,"logOutput","cout");
    vklCommitDevice(device);
}
//
// A little test code to validate the IO bits. 
// Open a particular file and create a Domain object.
//
int main(int argc, char **argv) {
    init_vkl();
    // the file spec
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("float.bov");
    std::string filespec = directory+filename;
    std::string fieldname("qi");
    std::cout << "Opening " + filespec + " " << std::endl;
    Domain Storm(filespec,fieldname);
    // VKLVolume StormVolume = DomainToVolume(Storm);
#ifdef WITH_NETCDF
    openvkl::testing::NetcdfStructuredVolume volume(filespec,fieldname);
#endif
    std::cout << "Done" << std::endl;
}   
