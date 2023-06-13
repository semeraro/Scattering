// camera impl
#include "camera.h"
camera::camera(vec2i resolution,float fov) {
    width = resolution.x;
    height = resolution.y;
    field_of_vision = deg2rad(fov);
}
void camera::set_fov(float fov) {
    field_of_vision = deg2rad(fov); //field of vision interally stored as radian.
}
void camera::set_resolution(vec2i resolution) {
    width  = resolution.x;
    height = resolution.y;
}
camera::~camera() {
    // destruct
}