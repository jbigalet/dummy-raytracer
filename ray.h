#ifndef DEF_RAY
#define DEF_RAY

#include <cfloat>

#include "vector.h"
#include "object.h"

class Ray {
  public:
    Vector orig; // origin
    Vector dir;  // direction
    Vector att;  // attenuation

    Ray(Vector orig, Vector dir, Vector att=VECTOR_ONE)
      : orig(orig), dir(dir), att(att) {};

    ~Ray() {};

    Vector point_at_parameter(float t) const { return orig + t*dir; }

    Vector color(Object &obj, int max_bounce);
};

#endif /* DEF_RAY */
