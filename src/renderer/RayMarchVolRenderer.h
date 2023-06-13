//
// RayMarchVolRenderer
// Given a Domain object this class creates the vkl objects needed to render pixels.
// And renders pixels.
//
#include "apps/AppInit.h"
#include "Ray.h"
#include "IO/Domain.h"
#include "camera.h"
#include <openvkl/openvkl.h>

class RayMarchVolRenderer {
    public:
    RayMarchVolRenderer(Domain &dom);
    ~RayMarchVolRenderer();
    void RenderPixel(Ray r, vec4f &rgba, float &weight );
    void setCameraResolution(vec2i resolution);
    void setCameraEyePoint(vec3f eye);
    void setCameraFocalPoint(vec3f fp);
    void setCameraUpVector(vec3f up);
    private:
    camera cam; // default camera size is 1024x768. fov = 60 degrees
    //Framebuffer fb;
    VKLVolume volume;
    VKLSampler sampler;
    vkl_range1f volumeValueRange;
    box3f dom_bounds;
    VKLIntervalIteratorContext intervalContext;
};