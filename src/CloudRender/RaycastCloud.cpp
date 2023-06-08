// simple raycast volume rendering
//
#include "apps/AppInit.h"
#include "rkcommon/utility/SaveImage.h"
#include <openvkl/openvkl.h>
#include "IO/Domain.h"
int main(int argc, char **argv) {
    
    std::cout << " Starting ... " << std::endl;
    initializeOpenVKL();
    // read the data
    // just reading the metadata
    // letting the openvkl volume read the raw data and build a volume.
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("cloud.bov");
    std::string filespec = directory+filename;
    // build a domain object
    Domain cloud = Domain(filespec,"qi",false);
    //generate a vklVolume
    VKLVolume vklCloud = DomainToVolume(cloud,getOpenVKLDevice());
    
    shutdownOpenVKL();
}