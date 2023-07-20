//
// first shot at volume scattering. 
// only using ambient light
//
#include "PathTraceRenderer.h"
PathTraceRenderer::PathTraceRenderer(Domain dom) : RayMarchVolRenderer(dom) { }
void PathTraceRenderer::RenderPixel(size_t seed,Ray &r, vec4f &rgba, float &weight) {

    RayMarchVolRenderer::RenderPixel(r,rgba,weight);
}
vec3f PathTraceRenderer::integrate(RNG &rng, Ray &r , int scatterIndex, int &maxScatterIndex) {
    return vec3f(1.0);
}
bool PathTraceRenderer::sampleWoodcock(RNG &rng, Ray &r, range1f &hits, float &t, float &sample, float &transmittance) {
    t = hits.lower;
    const float sigmaMax = sigmaTScale;
    if (sigmaMax <= 0.f) {
        transmittance = 1.0;
        return false;
    }
    while(true) {
        vec2f randomNumbers = rng.getFloats();
        vec2f randomNumbers2 = rng.getFloats();
        t = t+-std::log(1.f-randomNumbers.x)/sigmaMax;
        if(t>hits.upper) {
            transmittance = 1.f;
            return false;
        }
        const vec3f c = r.org + t*r.dir;
        sample = vklComputeSample(sampler,(const vkl_vec3f *)&c,0,0.f);
        vec4f sampleColorAndOpacity = sampleTF(sample);
        const float sigmaTSample = sigmaMax * sampleColorAndOpacity.w;
        if (randomNumbers.y < sigmaTSample / sigmaMax)
          break;
    }
    transmittance = 0.f;
    return true;
}
void PathTraceRenderer::RenderFrame() {
    std::cout << "pathtracer rendering" << std::endl;
    vec2i resolution = getCameraResolution();
    long numpixels = resolution.long_product();
    framebuffer.resize(numpixels);
    weightbuffer.resize(numpixels);
    auto startrender = Clock::now();
    rkcommon::tasking::parallel_in_blocks_of<16>(
      numpixels, [&](size_t ib, size_t ie) {
        Ray r;
        for (size_t idx = ib; idx < ie; ++idx) {
          int row = idx/resolution.x; // number of pixels from top of image (y)
          int column = idx % resolution.x; // number of pixels from left edge of image (x)
          r = cam.getRay(vec2i(column,row)); // expects x,y order
          r.t = intersectRayBox(r.org,r.dir,dom_bounds);
          vec4f &color = framebuffer[idx];
          float &wt = weightbuffer[idx];
          const size_t seed = resolution.x * static_cast<size_t>(row) + column;
          RenderPixel(seed,r,color,wt);
        }
      }
    );
}