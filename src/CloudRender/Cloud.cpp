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
#include "rkcommon/utility/SaveImage.h"
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
    // just reading the metadata
    // letting the openvkl volume read the raw data and build a volume.
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("cloud.bov");
    std::string filespec = directory+filename;
    //read the bov file 
    BOV_METADATA meta = read_bov_metadata(filespec);
    std::list<std::string> args(argv,argv+argc);
    // append domain data to args vector for use in volume construction
    args.push_back("-file");
    args.push_back(meta.rawfilename);
    args.push_back("-gridDimensions");
    args.push_back(std::to_string(meta.nx));
    args.push_back(std::to_string(meta.ny));
    args.push_back(std::to_string(meta.nz));
    args.push_back("-gridOrigin");
    args.push_back("0.0");
    args.push_back("0.0");
    args.push_back("0.0");
    args.push_back("-gridSpacing");
    args.push_back("1.0");
    args.push_back("1.0");
    args.push_back("1.0");
    args.push_back("-renderer");
    args.push_back("ray_march_iterator_ispc");
    // Let openvkl examples classes do their thing... 
    if(!scene.parseCommandLine(args)) {
        std::cout << "parseCommandLine yacked " << std::endl;
        return 0;
    } else {
        std::cout << " the arguments" << std::endl;
        for (std::string s : args) {
            std::cout << s << std::endl;
        }
    }
    scene.volume.updateVKLObjects();
    scene.volume.printInfo();
    //shutdownOpenVKL();
    scene.camera->fitToScreen(scenevolume.getBounds());
    scene.camera.incrementVersion();
    auto rendererPtr = scene.createRenderer("ray_march_iterator_ispc");
    //auto rendererPtr = scene.createRenderer("density_pathtracer_ispc");
    Renderer &renderer = *(rendererPtr.get());
        // This call will resize the framebuffer to our desired output
        // resolution.
    renderer.getFramebuffer(resolution.x, resolution.y);
    const std::string outfilename = "cloud.pfm";
    scheduler.start(renderer);
        for (unsigned i = 0; i < scene.batchModeSpp; ++i) {
          std::cout << "\r" << i << " / " << scene.batchModeSpp << " spp"
                    << std::flush;
          scheduler.renderFrame(renderer);
        }
        scheduler.stop(renderer);
        std::cout << std::endl;

        const auto &framebuffer =
            renderer.getFramebuffer(resolution.x, resolution.y);
        const auto &fb = framebuffer.getFrontBuffer();
        std::cout << "Writing " << outfilename << " ..." << std::endl;
        rkcommon::utility::writePFM(
            outfilename, fb.getWidth(), fb.getHeight(), fb.getRgba());
    std::cout << " ending ... " << std::endl;
}
