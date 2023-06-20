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

RayMarchVolRenderer::RayMarchVolRenderer(Domain &dom) : cam(Camera()) {
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
    vkl_range1f ranges[1] = {{volumeValueRange.lower, volumeValueRange.upper}};
    int num_ranges        = 1;
    VKLData rangesData =
      vklNewData(getOpenVKLDevice(), num_ranges, VKL_BOX1F, ranges, VKL_DATA_DEFAULT, 0);
    vklSetData(intervalContext, "valueRanges", rangesData);
    vklRelease(rangesData);
    vklCommit(intervalContext);
    transferfunction.setValueRange(vec2f(volumeValueRange.lower,volumeValueRange.upper));
};
RayMarchVolRenderer::~RayMarchVolRenderer() {
    vklRelease(sampler);
    vklRelease(volume);
    vklRelease(intervalContext);
};
void RayMarchVolRenderer::RenderFrame() {
    //vec2i resolution = cam.get_resolution();
    vec2i resolution = cam.filmsize;
    long numpixels = resolution.long_product();
    framebuffer.resize(numpixels);
    Ray r;
    int index;
    for(int row = 0;row<resolution.y;row++) {
      for(int column = 0;column<resolution.x;column++) {
        // get a ray from the camera
        index = row*resolution.x + column;
        r = cam.getRay(vec2i(column,row)); // column corresponds to the x coordinate row to the y from bottom left
        r.t = intersectRayBox(r.org,r.dir,dom_bounds);
        vec4f &color = framebuffer[index];
        float wt{0};
        RenderPixel(r,color,wt);
        //std::cout << color.x << " " << color.y << " " << color.z << " " << wt << std::endl;
      }
    }
}
// this bit is lifted from RayMarchIteratorRenderer in the openvkl examples
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
    vec3f color(0.f);
    float alpha = 0.f;
    while(vklIterateInterval(iterator,&interval)) {
        float sdt = interval.nominalDeltaT;
        // get a subinterval of this interval based on sdt.
        box1f subInterval(interval.tRange.lower,
                          min(interval.tRange.lower + sdt,
                              interval.tRange.upper));
        // now integrate along the subintervals

        intervals++;
        while(subInterval.upper > subInterval.lower && alpha < 0.99){
          // center of subinterval of size dt
          const float t  = 0.5f * (subInterval.lower + subInterval.upper);
          const float dt = subInterval.upper - subInterval.lower;
          // sample it
          vec3f c      = r.org + t * r.dir;
          float sample = vklComputeSample(sampler,
                                          (vkl_vec3f *)&c,
                                          0,
                                          0.f);
          // map through transfer function
          vec4f sampleColorAndOpacity = sampleTF(sample);

          // accumulate contribution
          const float clampedOpacity = clamp(sampleColorAndOpacity.w * dt);

          sampleColorAndOpacity = sampleColorAndOpacity * clampedOpacity;

          color = color + (1.f - alpha) * vec3f(sampleColorAndOpacity);
          alpha = alpha + (1.f - alpha) * clampedOpacity;
          // compute next sub interval
          subInterval.lower = subInterval.upper;
          subInterval.upper =
              min(subInterval.lower + sdt, interval.tRange.upper);
        }
          //printf(
           // "\t\tintervals %d tRange (%f %f)\n\t\tvalueRange (%f %f)\n\t\tnominalDeltaT "
           // "%f\n\n",intervals,
           // interval.tRange.lower,
           // interval.tRange.upper,
           // interval.valueRange.lower,
           // interval.valueRange.upper,
           // interval.nominalDeltaT);
    }
    rgba = vec4f(color,alpha);
};
void RayMarchVolRenderer::setCameraResolution(vec2i resolution) {
  cam.setFilmsize(resolution);
};
void RayMarchVolRenderer::setCameraEyePoint(vec3f eye) {
  cam.setPosition(eye);
};
void RayMarchVolRenderer::setCameraFocalPoint(vec3f fp) {
  cam.setFocus(fp);
};
void RayMarchVolRenderer::setCameraUpVector(vec3f up) {
  cam.setUp_Vector(up);
};
