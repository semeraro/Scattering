//
// A ray casting volume renderer created in the likeness of  
// the example renderer in the openvkl distribution.
//
// this thing takes a domain object, creates a few vkl objects and provides rendering services
// To render a pixel the corresponding ray is processed through an interval context corresponding
// to the desired value range of the volume attribute. Once intervals are obtained the integration along
// the ray proceeds and stops when you run out of intervals or the opacity reaches one. The rgba values
// for that pixel are then returned.
// Integration along the ray requires samples be mapped through a transfer function to obtain corresponding 
// rgba values. Most of the functionality is hidden internal to the class. Class usage consists of instantiation
// and setting of camera position, orientation and optics based on the global coordinate system associated with 
// the volume itself. The size of the framebuffer corresponds to the resolution of the camera. 

#include "RayMarchVolRenderer.h"

RayMarchVolRenderer::RayMarchVolRenderer(Domain &dom) : cam(camera(vec2i(1024,768))) {
    // create the vklvolume from the domain
    volume = DomainToVolume(dom,getOpenVKLDevice()); // DomainToVolume returns a committed volume. 
    volumeValueRange = vklGetValueRange(volume,0);
    dom_bounds = dom.bounds;
    // make a sampler
    sampler = vklNewSampler(volume);
    vklCommit(sampler);
    // intervalIteratorContext
    intervalContext = vklNewIntervalIteratorContext(sampler);
    vklSetInt(intervalContext, "attributeIndex", 0);
    vkl_range1f ranges[1] = {{0.000001, volumeValueRange.upper}};
    int num_ranges        = 1;
    VKLData rangesData =
      vklNewData(getOpenVKLDevice(), num_ranges, VKL_BOX1F, ranges, VKL_DATA_DEFAULT, 0);
    vklSetData(intervalContext, "valueRanges", rangesData);
    vklRelease(rangesData);
    vklCommit(intervalContext);
};
RayMarchVolRenderer::~RayMarchVolRenderer() {
    vklRelease(sampler);
    vklRelease(volume);
    vklRelease(intervalContext);
};
void RayMarchVolRenderer::RenderPixel(Ray r, vec4f &rgba, float &weight) {
    vkl_range1f tRange;
    tRange.lower = r.t.lower;
    tRange.upper = r.t.upper;
    void *buffer = alloca(vklGetIntervalIteratorSize(intervalContext));
    VKLIntervalIterator iterator =
          vklInitIntervalIterator(intervalContext,
                                  (vkl_vec3f *)&(r.org),
                                  (vkl_vec3f *)&(r.dir),
                                  &tRange,
                                  1.0,
                                  buffer);
    // sample the volume
    VKLInterval interval;
    int intervals = 0;
    while(vklIterateInterval(iterator,&interval)) {
        intervals++;
        printf(
          "\t\tintervals %d tRange (%f %f)\n\t\tvalueRange (%f %f)\n\t\tnominalDeltaT "
          "%f\n\n",intervals,
          interval.tRange.lower,
          interval.tRange.upper,
          interval.valueRange.lower,
          interval.valueRange.upper,
          interval.nominalDeltaT);
    }
};
void RayMarchVolRenderer::setCameraResolution(vec2i resolution) {
  cam.set_resolution(resolution);
};
void RayMarchVolRenderer::setCameraEyePoint(vec3f eye) {
  cam.set_eye_point(eye);
};
void RayMarchVolRenderer::setCameraFocalPoint(vec3f fp) {
  cam.set_focal_point(fp);
};
void RayMarchVolRenderer::setCameraUpVector(vec3f up) {
  cam.set_up_vector(up);
};
