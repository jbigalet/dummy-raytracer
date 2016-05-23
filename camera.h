#ifndef DEF_CAMERA
#define DEF_CAMERA 

#include <stdlib.h>
#include <cmath>

#include "stats.h"
#include "utils.h"

class Camera {
  public:
    int width, height;
    Vector orig, lower_left_corner, hor, ver;

    // vfov = vertical field of view, from top to bottom, in degrees
    // viewup is the 'up' vector of the camera
    Camera(int width, int height, Vector orig, Vector lookat, Vector viewup, float vfov)
           : width(width), height(height), orig(orig) {

      float theta = vfov*M_PI/180;
      float half_height = tan(theta/2);
      float half_width = width * half_height / height ;

      Vector w = (orig-lookat).unit();  // camera direction
      Vector u = (viewup*w).unit();
      Vector v = w*u;

      hor = 2*u*half_width;
      ver = 2*v*half_height;
      lower_left_corner = orig - (hor+ver)/2 - w;
    }

    ~Camera() {};

    /* Ray getRay(float u, float v){ */
    /*   return Ray(orig, lower_left_corner + u*hor + v*ver - orig); */
    /* } */

    /* Vector getColor(Object &obj, int x, int y, int max_bounce, int nsamples){ */
    /*   Vector color = VECTOR_ZERO; */
    /*   for(int i=0 ; i<nsamples ; i++) */
    /*     color += getRay( */
    /*                     (x+RANDOM_FLOAT)/width, */
    /*                     (y+RANDOM_FLOAT)/height */
    /*         ).color(obj, max_bounce); */

    /*   nDirectRay += nsamples;  // @stats */

    /*   return color/nsamples; */
    /* } */
};

#endif /* DEF_CAMERA */
