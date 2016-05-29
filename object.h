#ifndef DEF_OBJECT
#define DEF_OBJECT

#include <math.h>
#include <list>
#include <vector>

#include "vector.h"

class Ray;
class Material;

struct HitRecord {
  HitRecord(float t, Vector p, Vector normal, Material *material, float u, float v)
           : t(t), p(p), normal(normal), material(material), u(u), v(v) {};

  ~HitRecord() {
    /* delete material; */
  }

  float t;
  Vector p;
  Vector normal;
  Material *material;
  float u;
  float v;
};

class Object {
  public:
    ~Object() {}

    virtual HitRecord* hit(Ray ray, float t_min, float t_max) = 0;
};


class ObjectGroup : public Object {
  public:
    std::vector<Object*> list;

    ObjectGroup() {}

    ~ObjectGroup() {
      /* for(auto &it: list) */
      /*   delete it; */
      /* list.clear(); */
    }

    void add(Object *obj) { list.push_back(obj); }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      HitRecord *bestHit = NULL;
      for(Object* obj : list){
        HitRecord* hit = obj->hit(ray, t_min, bestHit == NULL ? t_max : bestHit->t);
        if(hit != NULL){
          delete bestHit;
          bestHit = hit;
        }
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
      float b = 2.0f * oc%ray.dir;
      float c = oc%oc - radius*radius;
      float discriminant = b*b-4.0f*a*c;

      float tmp;

      if(discriminant > 0.0f){
        float sqrtt = sqrt(discriminant);
        tmp = (-b-sqrtt)/(2.0f*a);

        if(tmp > t_min && tmp < t_max)
          goto returnHitRec;

        tmp = (-b+sqrtt)/(2.0f*a);
         /* tmp = -tmp - b/a;  // branch prediction makes this slower? why cant gcc optimize this.. */
        if(tmp > t_min && tmp < t_max)
          goto returnHitRec;
      }

      return NULL;


    returnHitRec:
      Vector hitpoint = ray.point_at_parameter(tmp);

      Vector norm = (hitpoint - center) / radius;
      if(norm%ray.dir > 0)
        norm = -norm;

      /* float u = 0.5f + atan2(dz, dx); */
      /* float v = 0.5f + asin(dy)/M_PI; */

      float phi = atan2(hitpoint.z, hitpoint.x);
      float theta = asin(hitpoint.y);

      float u = 1.f-(phi + M_PI)/(2*M_PI);
      float v = (theta + M_PI/2.f)/(M_PI);

      return new HitRecord(
          tmp,
          hitpoint,
          norm,
          material,
          u,
          v
      );
    }
};


class Plane: public Object {
  public:
    Vector point; // any point on the plane will do
    Vector norm;
    Material *material;

    Plane(Vector point, Vector norm, Material *material)
      : point(point), norm(norm), material(material) {}

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      float ND = norm%ray.dir;
      if(abs(ND) < 0.00001f) // assume its parallel
        return NULL;

      float t = (norm%(point-ray.orig))/ND;
      if(t > t_min && t < t_max)
        return new HitRecord(
          t,
          ray.point_at_parameter(t),
          norm,
          material,
          0.f,  // u
          0.f   // v
        );

      return NULL;
    }
};


class Triangle: public Object {
  public:
    Vector a, b, c;
    Vector norm;
    Material *material;

    Triangle(Vector a, Vector b, Vector c, Material *material)
      : a(a), b(b), c(c), material(material) {
      norm = ((b-a)*(c-a)).unit();
    }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      float ND = norm%ray.dir;

      if(ND > -0.0001f)   // TODO change that if we want dielectric triangles
        return NULL;

      /* if(abs(ND) < 0.00001f) // assume its parallel */
      /*   return NULL; */

      float t = (norm%(a-ray.orig))/ND;
      if(t > t_min && t < t_max) {
        // check if its inside the triangle
        // we check that the point in on the left side of each directed edges
        Vector hitpoint = ray.point_at_parameter(t);

        if(norm%((b-a)*(hitpoint-a)) < 0) return NULL;
        if(norm%((c-b)*(hitpoint-b)) < 0) return NULL;
        if(norm%((a-c)*(hitpoint-c)) < 0) return NULL;

        return new HitRecord(
            t,
            hitpoint,
            norm,
            material,
            0.f,  // u
            0.f   // v
            );
      }

      return NULL;
    }
};

#endif /* DEF_OBJECT */
