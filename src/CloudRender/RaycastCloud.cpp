// simple raycast volume rendering
//
#include "rkcommon/utility/SaveImage.h"
#include <openvkl/openvkl.h>
#include "renderer/RayMarchVolRenderer.h"
#include "renderer/PathTraceRenderer.h"
#include "renderer/Ray.h"
#include "IO/Domain.h"
#if defined(_MSC_VER)
#include <malloc.h>   // _malloca
#endif
#define useRayMarchRenderer
int main(int argc, char **argv) {
    
    std::cout << " Starting ... " << std::endl;
    initializeOpenVKL();
    int device_width = vklGetNativeSIMDWidth(getOpenVKLDevice());
    std::cout << "The openvkl device width is " << device_width << std::endl;
    // read the data
    // letting the openvkl volume read the raw data and build a volume.
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("cloud.bov");
    std::string outfilename("cloud.pfm");
    std::string filespec = directory+filename;
    // build a domain object
    Domain cloud = Domain(filespec,"qi",false);
    cloud.origin = vec3f(45.015,30.015,9.975); // in km
    cloud.spacing = vec3f(0.03,0.03,0.03); // 30 meter spacing
    vec3f lower = cloud.origin;
    vec3f upper = lower + cloud.spacing*(cloud.coords - vec3i(1,1,1));
    std::cout << "volume bounds " << lower << "\t" << upper << std::endl;
    vec3f center = (upper + lower)*0.5;
    cloud.bounds.lower = lower;
    cloud.bounds.upper = upper;
    vec3f dir = vec3f(0.,0.,-1.0);
    //vec3f org = vec3f(75.0,50.0,0.);
    vec3f org = center + vec3f(0.f,0.f,48.f + (upper - center ).z);
    // add a renderer
#ifdef useRayMarchRenderer
    //PathTraceRenderer ren = PathTraceRenderer(cloud);
    //ren.sigmaSScale = 100.0f;
    //ren.sigmaTScale = 3.0f;
    //ren.maxNumScatters = 256;
    RayMarchVolRenderer ren = RayMarchVolRenderer(cloud);
    ren.setCameraResolution(vec2i(512,512));
    vec2i camerares = ren.getCameraResolution();
    std::cout << "camera " << camerares.x << " " << camerares.y << std::endl;
    //place the camera
    ren.resetCamera(org,center,vec3f(0.f,1.f,0.f));
    std::cout << "Render frame " << std::endl;
    std::cout << "domain center " << center << " camera position " << org << " focus " << center << std::endl;
    ren.RenderFrame();
    std::cout << "Save Image " << std::endl;
    ren.SaveImage(outfilename);
    //ren.RenderPixel(r,rgba,wt);
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