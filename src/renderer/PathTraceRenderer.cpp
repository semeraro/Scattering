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
    return true;
}
void PathTraceRenderer::RenderFrame() {
    vec2i resolution = cam.filmsize;
    long numpixels = resolution.long_product();
    framebuffer.resize(numpixels);
    auto startrender = Clock::now();
}