// camera impl
#include "camera.h"
camera::camera(vec2i resolution,float fov) {
    width = resolution.x;
    height = resolution.y;
    field_of_vision = deg2rad(fov);
    eye_point = vec3f(0.,0.,0.); // initially at the origin
    focal_point = vec3f(0.,0.,-1.); // looking down the z axis
    up_vector = vec3f(0.,1.,0.); // y is up, x to the right. global coords
}
void camera::set_fov(float fov) {
    field_of_vision = deg2rad(fov); //field of vision interally stored as radian.
}
void camera::set_resolution(vec2i resolution) {
    width  = resolution.x;
    height = resolution.y;
}
void camera::set_eye_point(vec3f point) {
    eye_point = point;
}
void camera::set_focal_point(vec3f point) {
    focal_point = point;
}
void camera::set_up_vector(vec3f up_vec) {
    up_vector = up_vec;
}
camera::~camera() {
    // destruct
}