//
// A ray casting volume renderer modeled after 
// the example renderer in the openvkl distribution.
//

#include "RayMarchVolRenderer.h"

RayMarchVolRenderer::RayMarchVolRenderer(Domain &dom) : cam(camera(vec2i(1024,768))) {
    // create the vklvolume from the domain
    volume = DomainToVolume(dom,getOpenVKLDevice()); // DomainToVolume returns a committed volume. 
    volumeValueRange = vklGetValueRange(volume,0);
    // make a sampler
    sampler = vklNewSampler(volume);
    vklCommit(sampler);
    // intervalIteratorContext
    VKLIntervalIteratorContext intervalContext = vklNewIntervalIteratorContext(sampler);
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

};
