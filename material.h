#ifndef DEF_MATERIAL
#define DEF_MATERIAL

#include "ray.h"
#include "object.h"

class Material {
  public:
    virtual Ray scatter(Ray r, HitRecord rec);
};

#endif /* DEF_MATERIAL */
