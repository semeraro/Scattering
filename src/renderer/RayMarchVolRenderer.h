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
#include <rkcommon/tasking/parallel_for.h>
#include "TransferFunction.h"
#include <chrono>
#define PARALLEL
using Framebuffer = containers::AlignedVector<vec4f>;
using Weightbuffer = containers::AlignedVector<float>;
using Clock = std::chrono::high_resolution_clock;
class RayMarchVolRenderer {
    public:
    RayMarchVolRenderer(Domain &dom);
    ~RayMarchVolRenderer();
    void RenderPixel(Ray r, vec4f &rgba, float &weight );
    void setCameraResolution(vec2i resolution);
    void setCameraEyePoint(vec3f eye);
    void setCameraFocalPoint(vec3f fp);
    void setCameraUpVector(vec3f up);
    void resetCamera(vec3f eye, vec3f fp, vec3f up);
    void setTransferFunctionValueRange(vec2f range) {transferfunction.setValueRange(range);};
    void RenderFrame();
    void SaveImage(std::string filename) {
        rkcommon::utility::writePFM(
            filename, cam.filmsize.x, cam.filmsize.y, framebuffer.data());
    }
    vec2i getCameraResolution() { return cam.filmsize;}
    vec4f sampleTF(float value) { return transferfunction.sampleTF(value);}
    protected:
    Camera cam; // default camera size is 1024x768. fov = 60 degrees
    int device_width = vklGetNativeSIMDWidth(getOpenVKLDevice());
    //Framebuffer ;
    Framebuffer framebuffer;
    Weightbuffer weightbuffer;
    //TransferFunction
    TransferFunction transferfunction;
    //rkcommon::containers::AlignedVector<vec4f> framebuffer;
    VKLVolume volume;
    VKLSampler sampler;
    vkl_range1f volumeValueRange;
    box3f dom_bounds;
    VKLIntervalIteratorContext intervalContext;
    typedef std::chrono::duration<float, std::milli> duration;
    duration frametime{0};
    duration rendertime{0};
};