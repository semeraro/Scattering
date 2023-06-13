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
    void set_eye_point(vec3f point);
    void set_focal_point(vec3f point);
    void set_up_vector(vec3f up_vector);
    private:
    int width;
    int height;
    float field_of_vision; // height angle of view frustum
    vec3f eye_point;
    vec3f up_vector;
    vec3f focal_point;
};