#ifndef DEF_SHAPES_SPHERE
#define DEF_SHAPES_SPHERE

#include "../object.h"

class Sphere : public Object {
  public:
    Vector center;
    float radius;

    Sphere(Vector center, float radius) : center(center), radius(radius) {}

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

#endif /* DEF_SHAPES_SPHERE */
