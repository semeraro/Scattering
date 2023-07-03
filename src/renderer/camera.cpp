/**
 * camera implementation
 * A right handed camera. Physical screen coordinates -1.0 <= x,y, <= 1.0
 * Rays are constructed in camera space and transformed to world space.
 *
 */
#include "camera.h"
Camera::Camera(){
    // initialize variables
    position={0.f,0.f,0.f};
    focus = {0.f,0.f,-1.f};
    up_vector = {0.f,1.f,0.f};
    filmsize = {640,480};
    field_of_view = 30.f;
    tanfov = tan(deg2rad(field_of_view));
    d2fp = 0.5/tanfov;
    // derived quantities
    view_direction = normalize(focus - position);
    numRays = filmsize.long_product();
    //numRays = filmsize.x*filmsize.y;
    Rays.resize(numRays);
    // set the u,v,w vectors
    // w - unit vector from camera to focal point
    // u - unit vector perpendicular to w and up_vector
    // v - unit vector out top of camera
    w = view_direction; // direction from position to focus
    u = normalize(cross(w,up_vector)); // 
    v = normalize(cross(u,w)); // recast the up vector
    cam2wrld = linear3f(one);
    wrld2cam = linear3f(one);
}
Camera::Camera(const vec3f &p,const vec3f &f,const vec3f &u,const vec2i &fs) {
    position = p;
    focus = f;
    up_vector = u;
    filmsize = fs;
    field_of_view = 30.f;
    tanfov = tan(deg2rad(field_of_view));
    d2fp = 0.5/tanfov;
    numRays = fs.long_product();
    //numRays = fs.x*fs.y;
    view_direction = normalize(focus-position);
    aspect_ratio = (float)filmsize.x/(float)filmsize.y;
    UpdateCamera();
    Rays.resize(numRays);
}
// generate rays in row order from bottom left of screen 
// to top right of screen
void Camera::generateRays() {
    float x,y;
    for(int row = 0; row <filmsize.y;row++) 
        for(int col = 0;col<filmsize.x;col++){ //along the row
            int index = row*filmsize.x + col;
            //location of pixel in camera space
            y = ((float)row/(float)(filmsize.y-1) - 1.0f); // 
            x = aspect_ratio*((float)col/(float)(filmsize.x-1)-1.0f); // 
            Rays[index]= getRay(vec2f(x,y));
        }
}
Ray Camera::getRay(const vec2i &pixel) {
    Ray ray;
    float x,y;
    // coordinates in screen space
    y = tanfov*(2.0*(float)pixel.y/(float)(filmsize.y-1) - 1.0f); 
    x = tanfov*aspect_ratio*(2.0*(float)pixel.x/(float)(filmsize.x-1)-1.0f);
    ray.t = range1f(0.f,inf);
    ray.org = position;
    ray.dir = screen(vec2f(x,y));
    return ray;
}
vec3f Camera::screen(const vec2f &sp) {
    // given the coordinates of a point in screen space
    vec3f screen_position = vec3f(sp,1.f);
    //vec3f direction = w+(v*screen_position.y)+(u*screen_position.x);
    vec3f direction = cam2wrld*screen_position;
    //std::cout << "\nP: " << position << "\ndir: " << direction << "\nscreen_position: " << screen_position << std::endl;
    return normalize(direction);
}
Ray Camera::getRay(const vec2f &screen_position) {
    Ray ray;
    ray.t = range1f(0.f,inf);
    ray.org = position;
    ray.dir = screen(screen_position);
    return ray;
}

void Camera::UpdateCamera() { // update the camera internals
    w = view_direction; // direction from position to focus
    u = normalize(cross(w,up_vector)); // 
    v = normalize(cross(u,w)); // recast the up vector
    tanfov = tan(deg2rad(field_of_view));
    up_vector = v;
    buildTransform();
    std::cout << "\nW: " << w << "\nU: " << u << "\nV: " << v << std::endl;
    std::cout << "\nC2W: " << cam2wrld << std::endl;
    std::cout << "\nWorldDir: " << cam2wrld*vec3f(0.,0.,1.) << std::endl;
}

void Camera::setPosition(const vec3f &pos) {
    position = pos;
    view_direction = normalize(position - focus);
    UpdateCamera();
}

void Camera::setFocus(const vec3f &foc) {
    focus = foc;
    view_direction = normalize(focus - position);
    UpdateCamera();
}
void Camera::setUp_Vector(const vec3f &up) {
    up_vector = up;
    UpdateCamera();
}
void Camera::resetCamera(const vec3f &p,const vec3f &f,const vec3f &up) {
    position = p;
    focus = f;
    up_vector = up;
    view_direction = normalize(focus - position);
    UpdateCamera();
}
void Camera::setFilmsize(const vec2i &fs) {
    filmsize = fs;
    numRays = filmsize.x*filmsize.y;
    aspect_ratio = (float)filmsize.x/(float)filmsize.y;
    Rays.clear();
    Rays.resize(numRays);
}
void Camera::buildTransform() {
    // called from UpdateCamera to calculate new transforms
    // u,v,and w have been recalculated and are normalized in 
    // world space.
    cam2wrld = linear3f(u,v,w);
    wrld2cam = cam2wrld.inverse();
}
