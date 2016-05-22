#ifndef DEF_CAMERA
#define DEF_CAMERA 

#include <stdlib.h>

class Camera {
  public:
    int width, height;
    Vector orig, lower_left_corner, hor, ver;

    Camera(int width, int height, Vector orig, Vector lower_left_corner, Vector hor, Vector ver)
        : width(width), height(height),
          orig(orig), lower_left_corner(lower_left_corner),
          hor(hor), ver(ver) {}

    ~Camera() {};

    Ray getRay(float u, float v){
      return Ray(orig, lower_left_corner + u*hor + v*ver - orig);
    }

    Vector getColor(Object &obj, int x, int y, int max_bounce, int nsamples){
      Vector color = VECTOR_ZERO;
      for(int i=0 ; i<nsamples ; i++)
        color += getRay(
                        (x+drand48())/width,
                        (y+drand48())/height
            ).color(obj, max_bounce);

      return color/nsamples;
    }
};

#endif /* DEF_CAMERA */
