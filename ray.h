#ifndef DEF_RAY
#define DEF_RAY

#include <cfloat>

#include "vector.h"

class BHV;

class Ray {
  public:
    Vector orig; // origin
    Vector dir;  // direction
    Vector invdir;  // direction inverse - used in AABB intersection
    int sign[3];  // sign of dir
    mutable float t_min, t_max;  // ray min & max time
    float refract_v; // refractive indice


    Ray(Vector orig, Vector dir, float t_min=0.0001f, float t_max=FLT_MAX, float refract_v=1.f)
      : orig(orig), dir(dir), t_min(t_min), t_max(t_max), refract_v(refract_v) {
        invdir = Vector(1.f/dir.x,
                        1.f/dir.y,
                        1.f/dir.z);
        sign[0] = (invdir.x < 0);
        sign[1] = (invdir.y < 0);
        sign[2] = (invdir.z < 0);
      };

    ~Ray() {};

    Vector point_at_parameter(float t) const { return orig + t*dir; }

    Vector color(const BHV &obj, int max_bounce) const;
};

#endif /* DEF_RAY */
