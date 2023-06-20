#include "TransferFunction.h"
#include <fstream>
#include <iostream>
// default constructor creates white map with inverted opacity
TransferFunction::TransferFunction() {
    n_opacities = 2;
    n_colors = 2;
    //colormap.resize(n_colors);
    colormap.push_back(vec4f(0.f,1.f,1.f,1.f));
    colormap.push_back(vec4f(1.f,1.f,1.f,1.f));
    //opacitymap.resize(n_opacities);
    opacitymap.push_back(vec2f(0.f,0.f)); // transparent at 0
    opacitymap.push_back(vec2f(1.f,1.f)); // opaque at 1
    valueRange = { 0.0, 1.0 };
    length = 256;
    RGB = NULL;
    alpha = NULL;
    n_rgb=0;
    n_alpha=0;
    int i0 = 0, i1 = 1;
    float xmin = colormap[0].x ,xmax = colormap[n_colors-1].x;
    for (int i=0;i<length;i++) {
        float x = xmin +(i/float(length-1))*(xmax-xmin);
        if(x>xmax) x = xmax;
        while(colormap[i1].x < x ) i0++,i1++;
        float d = (x - colormap[i0].x) / (colormap[i1].x - colormap[i0].x);
        color[i].x = colormap[i0].y + d * (colormap[i1].y - colormap[i0].y);
        color[i].y = colormap[i0].z + d * (colormap[i1].z - colormap[i0].z);
        color[i].z = colormap[i0].w + d * (colormap[i1].w - colormap[i0].w);
        }
    i0 = 0, i1 = 1;
    xmin = opacitymap[0].x, xmax = opacitymap[n_opacities - 1].x;
    for (int i = 0; i < length; i++) {
        float x = xmin + (i / float(length-1)) * (xmax - xmin);
        if (x > xmax) x = xmax;
        while (opacitymap[i1].x < x) i0++, i1++;
        float d = (x - opacitymap[i0].x) / (opacitymap[i1].x - opacitymap[i0].x);
        opacity[i] = opacitymap[i0].y + d * (opacitymap[i1].y - opacitymap[i0].y);
        }
}
TransferFunction::~TransferFunction() {
}
void TransferFunction::load(std::string cname, std::string oname) {

    std::ifstream ifs;
    ifs.open(cname.c_str(), std::ios::in);
    if(ifs.fail()) 
        throw(std::string("error opening colormap file: ") + cname);
    int nc;
    ifs >> nc;
    n_colors = nc;
    colormap.resize(nc);
    vec4f c ;
    for(int i = 0;i<nc; i++) {
        ifs >> c.x >>c.y >> c.z >> c.w;
        colormap[i] = c;
    }
    ifs.close();

    ifs.open(oname.c_str(),std::ios::in);
    if(ifs.fail()) 
        throw(std::string("error opening opacity file: ") + oname);
    int no;
    ifs >> no;
    n_opacities = no;
    opacitymap.resize(no);
    vec2f o;
    for (int i=0;i<no;i++) {
        ifs >> o.x >> o.y;
        opacitymap[i] = o;
        std::cout << "opacity map " << opacitymap[i].x << " " << opacitymap[i].y << std::endl;
    }
    ifs.close();
    int i0 = 0, i1 = 1;
    float xmin = colormap[0].x ,xmax = colormap[n_colors-1].x;
    for (int i=0;i<length;i++) {
        float x = xmin +(i/float(length-1))*(xmax-xmin);
        if(x>xmax) x = xmax;
        while(colormap[i1].x < x ) i0++,i1++;
        float d = (x - colormap[i0].x) / (colormap[i1].x - colormap[i0].x);
        color[i].x = colormap[i0].y + d * (colormap[i1].y - colormap[i0].y);
        color[i].y = colormap[i0].z + d * (colormap[i1].z - colormap[i0].z);
        color[i].z = colormap[i0].w + d * (colormap[i1].w - colormap[i0].w);
    }
    i0 = 0, i1 = 1;
    xmin = opacitymap[0].x, xmax = opacitymap[n_opacities - 1].x;
    for (int i = 0; i < length; i++) {
        float x = xmin + (i / float(length-1)) * (xmax - xmin);
        if (x > xmax) x = xmax;
        while (opacitymap[i1].x < x) i0++, i1++;
        float d = (x - opacitymap[i0].x) / (opacitymap[i1].x - opacitymap[i0].x);
        opacity[i] = opacitymap[i0].y + d * (opacitymap[i1].y - opacitymap[i0].y);
        //std::cout << opacity[i] << std::endl;
                                  }
}
vec4f TransferFunction::sampleTF(float value) {
    vec4f colorAndOpacity{0.f};
    if(std::isnan(value)){
        return colorAndOpacity;
    }
    // map 0 < value <  1
    value =min(1.0f,(max(0.0f,(value - valueRange.x))/
        (valueRange.y - valueRange.x)))*(length-1.f);
    const int index = floor(value);
    const float remainder = value - index;
    vec3f color0 = color[index];
    vec3f color1 = color[min(index+1,(length-1))];
    vec3f samplevalue = (1.0-remainder)*color0 + remainder * color1;
    float opacity0 = opacity[index];
    float opacity1 = opacity[min(index+1,(length-1))];
    float sampleopacity = (1.0-remainder)*opacity0 + remainder * opacity1;
    colorAndOpacity = {samplevalue,sampleopacity};
    return colorAndOpacity;
}

