#pragma once

#include <string>
#include <vector>
#include "rkcommon/math/vec.h"

using namespace std;
using namespace rkcommon;
using namespace rkcommon::math;

class TransferFunction {
    public:
        TransferFunction();
        ~TransferFunction();
        void load(std::string cname, std::string oname);
        vec4f sampleTF(float sample);
        void setValueRange(vec2f range) {valueRange = range;
        std::cout << valueRange.x<<" "<< valueRange.y << std::endl;};
    protected:
        std::vector<vec4f> colormap;
        //vec4f *colormap;
        std::vector<vec2f> opacitymap;
        //vec2f *opacitymap;
        vec3f color[256];
        vec2f valueRange;
        float opacity[256];
        int n_colors, n_opacities;
        int length;
        vec3f *RGB;
        float *alpha;
        int n_rgb,n_alpha;
};
