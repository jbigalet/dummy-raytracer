#ifndef DEF_MATERIALS_LAMBERTIAN
#define DEF_MATERIALS_LAMBERTIAN

#include "../material.h"
#include "vector.h"

class Lambertian : public Material {
  public:
    Vector albedo;

    Lambertian(Vector albedo) : albedo(albedo) {};
    ~Lambertian() {};

    Ray scatter(Ray r, HitRecord rec){
      return Ray(rec.p, rec.normal + random_point_in_sphere(), albedo);
    }
};

#endif /* DEF_MATERIALS_LAMBERTIAN */
