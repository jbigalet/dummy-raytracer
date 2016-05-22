#ifndef DEF_OBJECT
#define DEF_OBJECT

#include <math.h>

#include "vector.h"
#include "object.h"

class Ray;

struct HitRecord {
  HitRecord(float t, Vector p, Vector normal): t(t), p(p), normal(normal) {};

  float t;
  Vector p;
  Vector normal;
};


class Object {
  public:
    virtual HitRecord* hit(Ray ray, float t_min, float t_max) = 0;
};


#endif /* DEF_OBJECT */
