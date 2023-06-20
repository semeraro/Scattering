/**
 * camera implementation
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
}
Camera::Camera(const vec3f &p,const vec3f &f,const vec3f &u,const vec2i &fs) {
    position = p;
    focus = f;
    up_vector = u;
    filmsize = fs;
    field_of_view = 30.f;
    numRays = fs.long_product();
    //numRays = fs.x*fs.y;
    view_direction = normalize(focus-position);
    UpdateCamera();
    Rays.resize(numRays);
}
// generate rays in row order from bottom left of screen 
// to top right of screen
void Camera::generateRays() {
    float aspect_ratio = (float)filmsize.x/(float)filmsize.y;
    float tanfov = tan(deg2rad(field_of_view));
    float x,y;
    for(int row = 0; row <filmsize.y;row++) 
        for(int col = 0;col<filmsize.x;col++){ //along the row
            int index = row*filmsize.x + col;
            //screen parameters in camera space
            y = tanfov*((float)row/(float)(filmsize.y-1) - 0.5f); // 
            x = aspect_ratio*tanfov*((float)col/(float)(filmsize.x-1)-0.5f); // 
            //Ray ray;
            //ray.t = range1f(0.f,inf);
            //ray.org = position;
            //ray.dir = screen(vec2f(x,y));
            //Rays[index]= ray;
            Rays[index]= getRay(vec2f(x,y));
        }
}
Ray Camera::getRay(const vec2i &pixel) {
    Ray ray;
    float x,y;
    float aspect_ratio = (float)filmsize.x/(float)filmsize.y;
    float tanfov = tan(deg2rad(field_of_view)); 
    y = tanfov*((float)pixel.y/(float)(filmsize.y-1) - 0.5f); 
    x = aspect_ratio*tanfov*((float)pixel.x/(float)(filmsize.x-1)-0.5f);
    ray.t = range1f(0.f,inf);
    ray.org = position;
    ray.dir = screen(vec2f(x,y));
    return ray;
}
vec3f Camera::screen(const vec2f &sp) {
    vec2f screen_position = sp;
    vec3f direction = w+(v*screen_position.y)+(u*screen_position.x);
    return direction;
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
    up_vector = v;
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
    UpdateCamera();
}
void Camera::setFilmsize(const vec2i &fs) {
    filmsize = fs;
    numRays = filmsize.x*filmsize.y;
    Rays.clear();
    Rays.resize(numRays);
}
