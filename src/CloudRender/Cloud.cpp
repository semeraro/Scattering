// 
// Scattering test code
//
// June, 2022
//
// Using classes from the OpenVKL/Examples where appropriate.
//
#include "renderer/Scene.h"
#include "IO/Domain.h"
#include <openvkl/openvkl.h>
using namespace openvkl::examples;
int main(int argc, char **argv) {
    
    std::cout << " Starting ... " << std::endl;
    initializeOpenVKL();
    Scene scene;
    // read the data
    std::list<std::string> args(argv,argv+argc);
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("cloud.bov");
    std::string filespec = directory+filename;
    std::string fieldname("qi");
    std::cout << "Opening " + filespec + " " << std::endl;
    Domain Storm(filespec,fieldname);
    VKLVolume vol = DomainToVolume(Storm,getOpenVKLDevice());
    //
    if(!scene.parseCommandLine(args)) {
        std::cout << "parseCommandLine yacked " << std::endl;
        return 0;
    } else {
        for (std::string s : args) {
            std::cout << s << std::endl;
        }
    }
    shutdownOpenVKL();
    std::cout << " ending ... " << std::endl;
}
