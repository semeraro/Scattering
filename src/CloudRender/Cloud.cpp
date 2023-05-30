// 
// Scattering test code
//
// June, 2022
//
// Using classes from the OpenVKL/Examples where appropriate.
//
#include "renderer/Scene.h"
#include "renderer/Framebuffer.h"
#include "renderer/Renderer.h"
#include "renderer/Scheduler.h"
#include "IO/Domain.h"
#include <openvkl/openvkl.h>
using namespace openvkl::examples;
int main(int argc, char **argv) {
    
    std::cout << " Starting ... " << std::endl;
    initializeOpenVKL();
    Scene scene;
    auto &scheduler = scene.scheduler;
    auto &scenevolume = scene.volume;
    scene.rendererParams->fixedFramebufferSize = true;
    const vec2i resolution = scene.rendererParams->framebufferSize;

    // read the data
    //std::string directory("C:\\Users\\Dave\ Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string directory("C:\\");
    std::string filename("mkow030_qi_305x3001x2501_float.raw");
    std::string filespec = directory+filename;
    std::list<std::string> args(argv,argv+argc);
    args.push_back("-file");
    args.push_back(filespec);
    args.push_back("-gridDimensions");
    args.push_back("2501");
    args.push_back("3001");
    args.push_back("305");
    args.push_back("-gridOrigin");
    args.push_back("0.0");
    args.push_back("0.0");
    args.push_back("0.0");
    args.push_back("-gridSpacing");
    args.push_back("1.0");
    args.push_back("1.0");
    args.push_back("1.0");
    //for (std::string s : args) {
    //        std::cout << s << std::endl;
    //    }
    std::string fieldname("qi");
    std::cout << "Opening " + filespec + " " << std::endl;
    //Domain Storm(filespec,fieldname);
    //
    
    if(!scene.parseCommandLine(args)) {
        std::cout << "parseCommandLine yacked " << std::endl;
        return 0;
    } else {
        std::cout << " the arguments" << std::endl;
        
        for (std::string s : args) {
            std::cout << s << std::endl;
        }
    }
    //VKLVolume vol = DomainToVolume(Storm,getOpenVKLDevice(),scene.volume.getVolume());
    scene.volume.updateVKLObjects();
    scene.volume.printInfo();
    shutdownOpenVKL();
    std::cout << " ending ... " << std::endl;
}
