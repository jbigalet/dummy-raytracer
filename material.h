#ifndef DEF_MATERIAL
#define DEF_MATERIAL

#include "ray.h"
#include "vector.h"
#include "object.h"


class Material {
  public:
    ~Material() {}
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

class Metal: public Material {
  public:
    Vector albedo;
    float fuzz;

    Metal(Vector albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {};
    ~Metal() {};

    Ray *scatter(Ray &r, HitRecord &rec){
      Ray *res = new Ray(rec.p, r.dir.unit() | (rec.normal + fuzz*random_point_in_sphere()), albedo);
      if( res->dir % rec.normal > 0 )
        return res;
      return NULL;
    }
};


#endif /* DEF_MATERIAL */
