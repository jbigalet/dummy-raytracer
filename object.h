#ifndef DEF_OBJECT
#define DEF_OBJECT

#include <math.h>

#include "vector.h"

class Ray;

struct HitRecord {
  HitRecord(float t, Vector p, Vector normal): t(t), p(p), normal(normal) {};

  float t;
  Vector p;
  Vector normal;
};

class Material;

class Object {
  public:
    Material *material;
    virtual HitRecord* hit(Ray ray, float t_min, float t_max) = 0;
};


// Implementations


class Sphere : public Object {
  public:
    Vector center;
    float radius;

    Sphere(Vector center, float radius, Material *mat)
      : center(center), radius(radius) {
        material = mat;
      }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      Vector oc = ray.orig - center;
      float a = ray.dir%ray.dir;
      float b = 2.0 * oc%ray.dir;
      float c = oc%oc - radius*radius;
      float discriminant = b*b-4*a*c;

      if(discriminant > 0){
        float tmp = (-b-sqrt(discriminant))/(2.0*a);
        if(tmp > t_min && tmp < t_max)
          return new HitRecord(
              tmp,
              ray.point_at_parameter(tmp),
              (ray.point_at_parameter(tmp) - center) / radius
              );

        tmp = (-b+sqrt(discriminant))/(2.0*a);
        if(tmp > t_min && tmp < t_max)
          return new HitRecord(
              tmp,
              ray.point_at_parameter(tmp),
              (ray.point_at_parameter(tmp) - center) / radius
              );
      }

      return NULL;
    }
};


#endif /* DEF_OBJECT */
