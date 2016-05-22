#ifndef DEF_MATERIAL
#define DEF_MATERIAL

#include "object.h"
#include "ray.h"

class Material {
  public:
    virtual Ray *scatter(Ray &r, HitRecord &rec) = 0;
};

#endif /* DEF_MATERIAL */
