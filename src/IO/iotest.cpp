/*
 *
 */
#include "Domain.h"
#include "NetcdfStructuredVolume.h"
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
    std::string directory("/home1/01197/semeraro/CM1/ORFSCRATCH/2022/DaveS/old/");
    std::string filename("mkow030-bigdomain-uncompressed.00252000.nc");
    std::string filespec = directory+filename;
    std::string fieldname("qi");
    std::cout << "Opening " + filespec + " " << std::endl;
    // Domain Storm(filespec,fieldname);
    // VKLVolume StormVolume = DomainToVolume(Storm);
    openvkl::testing::NetcdfStructuredVolume volume(filespec,fieldname);
    std::cout << "Done" << std::endl;
}   
