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

class Dielectric: public Material {
  public:
    float refract_int;
    float refract_ext;

    Dielectric(float rint, float rext=1.f): refract_int(rint), refract_ext(rext) {};
    ~Dielectric() {};

    Ray *scatter(Ray &r, HitRecord &rec){

      // 2 cases: either we're currently 'inside' this mat, so refraction leaves it
      // or we're outside & refraction goes inside
      float futur_refract;  // next refraction indice
      if(r.refract_v == refract_int)  // <=> inside, going out
        futur_refract = refract_ext;
      else
        futur_refract = refract_int;

      // compute refraction / reflection ratio - see fresnel equations
      // using schlick's approximation (cf wikipedia)
      float r0sqrt = (r.refract_v - futur_refract) / (r.refract_v + futur_refract);
      float r0 = r0sqrt*r0sqrt;
      float cos = -r.dir.unit() % rec.normal;
      float coef = r0 + (1.f-r0)*pow(1.f-cos, 5.f);

      if(RANDOM_FLOAT > coef){ // refraction
        Vector* refracted = refract(r.dir.unit(), rec.normal, r.refract_v/futur_refract);
        if(refracted != NULL)
          return new Ray(rec.p, *refracted, VECTOR_ONE, futur_refract);
      }

      // either no refraction or we chose to pick the refraction with schlick's app:
      return new Ray(rec.p, r.dir.unit() | rec.normal);
    }
};


#endif /* DEF_MATERIAL */
