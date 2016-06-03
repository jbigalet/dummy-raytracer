#include "ray.h"
#include "material.h"
#include "object.h"

#include <atomic>
extern std::atomic<long> nTotalRay;

Vector Ray::color(Object &obj, int max_bounce){
  nTotalRay++;  // @stats

  if(max_bounce == 0)
    return VECTOR_ZERO;

  HitRecord *hit = obj.hit(*this, 0.0001f, FLT_MAX);

  if(hit != NULL){
    /* return 0.5*Vector(hit->normal.x+1, hit->normal.y+1, hit->normal.z+1); */
    Vector attenuation;
    Ray *scattered = hit->material->scatter(*this, *hit, attenuation);

    delete hit;

    if(scattered != NULL && attenuation.length() > 0.01f) {
    /* if(scattered != NULL) { */
      Vector res = scattered->color(obj, max_bounce-1) ^ attenuation;
      delete scattered;
      return res;
    } else {
      return attenuation;
    }

  }

  // sky
  /* float t = 0.5f*(dir.unit().y + 1.0f); */
  /* return (1.0-t)*VECTOR_ONE + t*Vector(0.5f, 0.7f, 1.0f); */

  // void
  return VECTOR_ZERO;
}

