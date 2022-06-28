// 
// Scattering test code
//
// June, 2022
//
// Using classes from the OpenVKL/Examples where appropriate.
//
#include "renderer/Scene.h"
using namespace openvkl::examples;

int main(int argc, char **argv) {
    
    std::cout << " Starting ... " << std::endl;
    initializeOpenVKL();
    Scene scene;
    std::list<std::string> args(argv,argv+argc);
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
