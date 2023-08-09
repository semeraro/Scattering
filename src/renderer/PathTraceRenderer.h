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
// couple of useful functions
// -------------------------------------------------------------------------

    static vec3f cartesian(const float phi,
                           const float sinTheta,
                           const float cosTheta)
    {
      float sinPhi = std::sin(phi);
      float cosPhi = std::cos(phi);
      return vec3f(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta);
    }

    static vec3f uniformSampleSphere(const float radius, const vec2f &s)
    {
      const float phi      = float(two_pi) * s.x;
      const float cosTheta = radius * (1.f - 2.f * s.y);
      const float sinTheta = 2.f * radius * std::sqrt(s.y * (1.f - s.y));
      return cartesian(phi, sinTheta, cosTheta);
    }

// -------------------------------------------------------------------------
// And the Renderer
class PathTraceRenderer : public RayMarchVolRenderer {
    public:
        PathTraceRenderer(Domain dom);
        // these variables are stored in a parameters object in the original
        // ospray examples code. I want them here in the path tracer. 
        // they need to be initialized in the constructor. changing
        // the values will yield different rendering behavior. 
        float sigmaTScale;
        float sigmaSScale;
        float ambientLightIntensity;
        int maxNumScatters;
        //~PathTraceRenderer();
        // methods
        void RenderFrame();
        void RenderPixel(size_t seed, Ray &r, vec4f &rgba, float &weight);
        vec3f integrate(RNG &rng, Ray &r , int scatterIndex, int &maxScatterIndex) ;
        bool sampleWoodcock(RNG &rng, Ray &r, range1f &hits, float &t, float &sample, float &transmittance );
};