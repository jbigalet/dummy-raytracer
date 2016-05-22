#include "ray.h"
#include "material.h"

Vector Ray::color(Object &obj, int max_bounce){
  if(max_bounce == 0)
    return VECTOR_ZERO;

    HitRecord *hit = obj.hit(*this, 0.001f, FLT_MAX);

    if(hit != NULL){
      Ray *scattered = obj.material->scatter(*this, *hit);
      if(scattered != NULL)
        return scattered->color(obj, max_bounce-1) * scattered->att;
      else
        return VECTOR_ZERO;
    }

  float t = 0.5*(dir.unit().y + 1.0);
  return (1.0-t)*VECTOR_ONE + t*Vector(0.5, 0.7, 1.0);
}

