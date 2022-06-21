// 
// Scattering test code
//
// June, 2022
//
// Using classes from the OpenVKL/Examples.
//
#include "renderer/Scene.h"
using namespace openvkl::examples;

int main(int argc, char **argv) {
    
    Scene scene;
    std::list<std::string> args(argv,argv+argc);
    if(!scene.parseCommandLine(args)) {
        return 0;
    }
}
