/**
 * Camera class. 
 * This class contains all knowledge of the view and image size.
 * The actual image is contained in a framebuffer object.
 * The purpose of this class is to generate rays.
 * The calculations suppose a screen plane one unit from the camera position
 * orthogonal to the w vector. The v vector is the vertical direction of the screen.
 * v points out the top of the camera. The u direction is the horizontal direction. u,v,w form a 
 * left handed coordinate system of unit vectors. 
 */
#pragma once
#include <rkcommon/math/vec.h>
#include <rkcommon/math/LinearSpace.h>
#include "ray.h"
#include <vector>
#include <iostream>
//
//  simple camera class
//
using namespace rkcommon;
using namespace rkcommon::math;
class Camera {
    public:
        Camera();
        Camera(const vec3f &p,const vec3f &f,const vec3f &u,const vec2i &fs);
        void setPosition(const vec3f &pos);  // change the position of the camera
        void setFocus(const vec3f &foc);     // change the focal point
        void setUp_Vector(const vec3f &up);
        void setFilmsize(const vec2i &fs);
        void generateRays();
        void UpdateCamera();
        void buildTransform();
        void resetCamera(const vec3f &p,const vec3f &f,const vec3f &up);
        RayVector &getRays() { return Rays; }
        vec3f screen(const vec2f &sp); // returns direction from camera origin to position on the screen.
        Ray getRay(const vec2f &screen_position); // returns a ray from camera origin to position on the screen. 
        Ray getRay(const vec2i &pixel);
        //friend std::ostream & operator << (std::ostream &out, const Camera &C);
    public:
        // camera definitions
        vec3f position; // where the camera is in world space
        vec3f focus;    // the point the camer looks at in world space
        vec3f up_vector;// unit vector pointing out the top of camera
        vec3f view_direction; // focus - position
        vec3f u,v,w; // unit basis vectors for camera in world coords.
        // film definitions
        vec2i filmsize; // (width,height) pixels in each direction
        linear3f cam2wrld;
        linear3f wrld2cam;
        float aspect_ratio; 
        float field_of_view; // vertical half angle
        float tanfov;
        float d2fp; // distance to film plane
        long numRays; // number of primary rays
        RayVector Rays; // the rays
};
//std::ostream & operator << (std::ostream &out, const Camera &C) {
            //out << "Eye: " << C.position << "\nFocal Pt: " << C.focus;
           // out << "\nUp Vector: " << C.up_vector << "\nFOV: " << C.field_of_view;
           // return out;        
     // }
