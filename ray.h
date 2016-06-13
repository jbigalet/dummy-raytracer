#ifndef DEF_RAY
#define DEF_RAY

#include <cfloat>

#include "vector.h"

class Object;

class Ray {
  public:
    Vector orig; // origin
    Vector dir;  // direction
    Vector invdir;  // direction inverse - used in AABB intersection
    int sign[3];  // sign of dir
    float refract_v; // refractive indice

    Ray(Vector orig, Vector dir, float refract_v=1.f)
      : orig(orig), dir(dir), refract_v(refract_v) {
        invdir = Vector(1.f/dir.x,
                        1.f/dir.y,
                        1.f/dir.z);
        sign[0] = (invdir.x < 0);
        sign[1] = (invdir.y < 0);
        sign[2] = (invdir.z < 0);
      };

    ~Ray() {};

    Vector point_at_parameter(float t) const { return orig + t*dir; }

    Vector color(Object &obj, int max_bounce);
};

#endif /* DEF_RAY */
