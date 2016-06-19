#include "ray.h"
#include "material.h"
#include "object.h"
#include "stats.h"

Vector Ray::color(Object &obj, int max_bounce){
  nTotalRay++;  // @stats

  if(max_bounce == 0)
    return VECTOR_ZERO;

  HitRecord hit;
  if(obj.hit(*this, hit)){
    // debug: normal map
    /* return 0.5*Vector(hit->normal.x+1, hit->normal.y+1, hit->normal.z+1); */

    Vector attenuation;
    Ray *scattered = hit.material->scatter(*this, hit, attenuation);

    if(scattered != NULL) {
      if(attenuation.length() > 0.01f){
        Vector res = scattered->color(obj, max_bounce-1) ^ attenuation;
        delete scattered;
        return res;
      } else {
        delete scattered;
      }
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

