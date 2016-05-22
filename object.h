#ifndef DEF_OBJECT
#define DEF_OBJECT

#include <math.h>
#include <list>
#include <vector>

#include "vector.h"

class Ray;
class Material;

struct HitRecord {
  HitRecord(float t, Vector p, Vector normal, Material *material)
           : t(t), p(p), normal(normal), material(material) {};

  float t;
  Vector p;
  Vector normal;
  Material *material;
};

class Object {
  public:
    virtual HitRecord* hit(Ray ray, float t_min, float t_max) = 0;
};


class ObjectGroup : public Object {
  public:
    std::vector<Object*> list;

    ObjectGroup() {}
    ~ObjectGroup() {}

    void add(Object *obj) { list.push_back(obj); }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      HitRecord *bestHit = NULL;
      for(Object* obj : list){
        HitRecord* hit = obj->hit(ray, t_min, bestHit == NULL ? t_max : bestHit->t);
        if(hit != NULL)
          bestHit = hit;
      }
      return bestHit;
    }
};


// Implementations

class Sphere : public Object {
  public:
    Vector center;
    float radius;
    Material *material;

    Sphere(Vector center, float radius, Material *material)
      : center(center), radius(radius), material(material) {}

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
              (ray.point_at_parameter(tmp) - center) / radius,
              material
              );

        tmp = (-b+sqrt(discriminant))/(2.0*a);
        if(tmp > t_min && tmp < t_max)
          return new HitRecord(
              tmp,
              ray.point_at_parameter(tmp),
              (ray.point_at_parameter(tmp) - center) / radius,
              material
              );
      }

      return NULL;
    }
};


#endif /* DEF_OBJECT */
