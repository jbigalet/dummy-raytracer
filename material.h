#ifndef DEF_MATERIAL
#define DEF_MATERIAL

#include "ray.h"
#include "vector.h"
#include "object.h"


class Material {
  public:
    virtual Ray *scatter(Ray &r, HitRecord &rec) = 0;
};


// Implementations


class Lambertian : public Material {
  public:
    Vector albedo;

    Lambertian(Vector albedo) : albedo(albedo) {};
    ~Lambertian() {};

    Ray *scatter(Ray &r, HitRecord &rec){
      return new Ray(rec.p, rec.normal + random_point_in_sphere(), albedo);
    }
};


#endif /* DEF_MATERIAL */
