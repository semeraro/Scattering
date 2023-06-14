// simple raycast volume rendering
//
#include "rkcommon/utility/SaveImage.h"
#include <openvkl/openvkl.h>
#include "renderer/RayMarchVolRenderer.h"
#include "renderer/Ray.h"
#include "IO/Domain.h"
#if defined(_MSC_VER)
#include <malloc.h>   // _malloca
#endif
#define useRayMarchRenderer
int main(int argc, char **argv) {
    
    std::cout << " Starting ... " << std::endl;
    initializeOpenVKL();
    // read the data
    // letting the openvkl volume read the raw data and build a volume.
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("cloud.bov");
    std::string filespec = directory+filename;
    // build a domain object
    Domain cloud = Domain(filespec,"qi",false);
    cloud.origin = vec3f(45.015,30.015,9.975); // in km
    cloud.spacing = vec3f(0.03,0.03,0.03); // 30 meter spacing
    vec3f lower = cloud.origin;
    vec3f upper = lower + cloud.spacing*(cloud.coords - vec3i(1,1,1));
    std::cout << lower << "\t" << upper << std::endl;
    cloud.bounds.lower = lower;
    cloud.bounds.upper = upper;
    vec3f org = vec3f(75.0,50.0,20.0);
    vec3f dir = vec3f(0.,0.,-1.0);
    // add a renderer
#ifdef useRayMarchRenderer
    RayMarchVolRenderer ren = RayMarchVolRenderer(cloud);
    ren.setCameraEyePoint(org);
    ren.setCameraFocalPoint(org+dir);
    Ray r;
    r.org = org;
    r.dir = dir;
    auto t = intersectRayBox(org,dir,cloud.bounds);
    r.t = t;
    vec4f rgba{0.,0.,0.,1.};
    float wt;
    ren.RenderPixel(r,rgba,wt);
#else
    // generate a vklVolume
    VKLVolume vklCloud = DomainToVolume(cloud,getOpenVKLDevice());
    vkl_range1f valueRange = vklGetValueRange(vklCloud, 0);
    printf("\tvalue range (attribute %u) = (%f %f)\n",
           0,
           valueRange.lower,
           valueRange.upper);
    // create a sampler
    VKLSampler sampler = vklNewSampler(vklCloud);
    vklCommit(sampler);
    // create interval context
    VKLIntervalIteratorContext intervalContext = vklNewIntervalIteratorContext(sampler);
    vklSetInt(intervalContext, "attributeIndex", 0);
    vkl_range1f ranges[1] = {{0.000001, valueRange.upper}};
    int num_ranges        = 1;
    VKLData rangesData =
      vklNewData(getOpenVKLDevice(), num_ranges, VKL_BOX1F, ranges, VKL_DATA_DEFAULT, 0);
    void *buffer = alloca(vklGetIntervalIteratorSize(intervalContext));
    vklSetData(intervalContext, "valueRanges", rangesData);
    vklRelease(rangesData);
    vklCommit(intervalContext);
    auto t = intersectRayBox(org,dir,cloud.bounds);
    vkl_range1f tRange;
    tRange.lower = t.lower;
    tRange.upper = t.upper;
    VKLIntervalIterator iterator =
          vklInitIntervalIterator(intervalContext,
                                  (vkl_vec3f *)&org,
                                  (vkl_vec3f *)&dir,
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
#endif
    shutdownOpenVKL();
}