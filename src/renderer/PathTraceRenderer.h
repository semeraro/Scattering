//
//  Path traced volume renderer 
//  extends RayMarchVolRenderer
//  borrows from the density path tracer in the openvkl examples
// initial version follows that model of ambient light source 
//
#include "RayMarchVolRenderer.h"
//
// some random number foo
#include <random>
inline float getRandomUniform()
    {
      static thread_local std::minstd_rand rng;
      static std::uniform_real_distribution<float> distribution{0.f, 1.f};

      return distribution(rng);
    }

    struct RandomSTD
    {
      template <typename T>
      RandomSTD(const T &, const T &)
      {
      }

      vec2f getFloats()
      {
        return vec2f{getRandomUniform(), getRandomUniform()};
      }
    };

    using RNG = RandomSTD;
class PathTraceRenderer : public RayMarchVolRenderer {
    public:
        PathTraceRenderer(Domain dom);
        //~PathTraceRenderer();
        void RenderFrame();
        void RenderPixel(size_t seed, Ray &r, vec4f &rgba, float &weight);
        vec3f integrate(RNG &rng, Ray &r , int scatterIndex, int &maxScatterIndex);
        bool sampleWoodcock(RNG &rng, Ray &r, range1f &hits, float &t, float &sample, float &transmittance );
};