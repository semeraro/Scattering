//
// RayMarchVolRenderer
// Given a Domain object this class creates the vkl objects needed to render pixels.
// And renders pixels.
// this class maintains a simple framebuffer
//
#pragma once
#include "apps/AppInit.h"
#include "Ray.h"
#include "IO/Domain.h"
#include "camera.h"
#include <openvkl/openvkl.h>
#include "rkcommon/containers/AlignedVector.h"
#include "rkcommon/utility/SaveImage.h"
#include "TransferFunction.h"
using Framebuffer = containers::AlignedVector<vec4f>;
class RayMarchVolRenderer {
    public:
    RayMarchVolRenderer(Domain &dom);
    ~RayMarchVolRenderer();
    void RenderPixel(Ray r, vec4f &rgba, float &weight );
    void setCameraResolution(vec2i resolution);
    void setCameraEyePoint(vec3f eye);
    void setCameraFocalPoint(vec3f fp);
    void setCameraUpVector(vec3f up);
    void setTransferFunctionValueRange(vec2f range) {transferfunction.setValueRange(range);};
    void RenderFrame();
    void SaveImage(std::string filename) {
        rkcommon::utility::writePFM(
            filename, cam.filmsize.x, cam.filmsize.y, framebuffer.data());
    }
    vec2i getCameraResolution() { return cam.filmsize;}
    vec4f sampleTF(float value) { return transferfunction.sampleTF(value);}
    private:
    Camera cam; // default camera size is 1024x768. fov = 60 degrees
    //Framebuffer ;
    Framebuffer framebuffer;
    //TransferFunction
    TransferFunction transferfunction;
    //rkcommon::containers::AlignedVector<vec4f> framebuffer;
    VKLVolume volume;
    VKLSampler sampler;
    vkl_range1f volumeValueRange;
    box3f dom_bounds;
    VKLIntervalIteratorContext intervalContext;
};