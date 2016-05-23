#include "ray.h"
#include "material.h"
#include "object.h"

extern int nTotalRay;

Vector Ray::color(Object &obj, int max_bounce){
  nTotalRay++;  // @stats

  if(max_bounce == 0)
    return VECTOR_ZERO;

  HitRecord *hit = obj.hit(*this, 0.0001f, FLT_MAX);

  if(hit != NULL){
    /* return 0.5*Vector(hit->normal.x+1, hit->normal.y+1, hit->normal.z+1); */
    Ray *scattered = hit->material->scatter(*this, *hit);

    delete hit;

    if(scattered != NULL) {
      Vector res = scattered->color(obj, max_bounce-1) ^ scattered->att;
      delete scattered;
      return res;
    } else {
      return VECTOR_ZERO;
    }

  }

  float t = 0.5f*(dir.unit().y + 1.0f);
  return (1.0-t)*VECTOR_ONE + t*Vector(0.5f, 0.7f, 1.0f);
}

