//
// simple pinhole camera
//
#include "rkcommon/math/vec.h"
using namespace rkcommon::math;
class camera {
    public:
    camera(vec2i resolution,float fov=60.0);
    ~camera();
    void set_resolution(vec2i resolution);
    void set_fov(float fov);
    private:
    int width;
    int height;
    float field_of_vision; // height angle of view frustum

};