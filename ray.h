#ifndef DEF_RAY
#define DEF_RAY

#include <cfloat>

#include "vector.h"

class Object;

class Ray {
  public:
    Vector orig; // origin
    Vector dir;  // direction
    float refract_v; // refractive indice

    Ray(Vector orig, Vector dir, float refract_v=1.f)
      : orig(orig), dir(dir), refract_v(refract_v) {};

    ~Ray() {};

    Vector point_at_parameter(float t) const { return orig + t*dir; }

    Vector color(Object &obj, int max_bounce);
};

#endif /* DEF_RAY */
