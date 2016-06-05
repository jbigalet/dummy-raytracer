#ifndef DEF_OBJECT
#define DEF_OBJECT

#include <math.h>
#include <list>
#include <vector>
#include <algorithm>

#include "vector.h"
#include "stats.h"

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


class AABB;

class Object {
  public:
    ~Object() {}

    virtual HitRecord* hit(Ray ray, float t_min, float t_max) = 0;
    virtual AABB* bounding_box() = 0;

    virtual inline std::string str(std::string indent="") {
      return indent + "<object>";
    }

    virtual inline int depth() {
      return 1;
    }
};


// axis aligned bounding box
class AABB : public Object {
  public:
    Vector vmin;
    Vector vmax;

    AABB() {};
    AABB(Vector vmin, Vector vmax) : vmin(vmin), vmax(vmax) {}

    inline AABB operator&(const AABB &box) {
      return AABB(
          Vector(
            fmin(vmin.x, box.vmin.x),
            fmin(vmin.y, box.vmin.y),
            fmin(vmin.z, box.vmin.z)
            ),

          Vector(
            fmax(vmax.x, box.vmax.x),
            fmax(vmax.y, box.vmax.y),
            fmax(vmax.z, box.vmax.z)
            )
          );
    }


    AABB* bounding_box(){
      return this;
    }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      for(int i=0 ; i<3 ; i++){
        float t0 = (vmin[i] - ray.orig[i])/ray.dir[i];
        float t1 = (vmax[i] - ray.orig[i])/ray.dir[i];
        if(ray.dir[i] < 0.f)
          std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if(t_max <= t_min - 0.001f)  // epsilon to avoid some floating point rounding stuff 'hiding' some axis aligned triangles
          return NULL;
      }

      // we dont care about lots of stuff since we'll only be using this in our BHV
      // TODO replace this with a dummy constant, or boolean returning function
      return new HitRecord(
          0.0f,
          VECTOR_ZERO,
          VECTOR_ZERO,
          NULL,
          0.0f,
          0.0f
      );
    }

    inline std::string str(std::string indent="") {
      return indent + "AABB:\n"
              + indent + "  min: " + vmin.str()
              + indent + "  max: " + vmax.str();
    }
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

    void add(Object *obj) {
      list.push_back(obj);
    }

    void extend(std::vector<Object*> toappend) {
      list.insert(list.end(), toappend.begin(), toappend.end());
    }

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

    AABB* bounding_box(){
      return NULL;  // TODO? we dont really care atm
    }

    inline std::string str(std::string indent="") {
      return indent + "ObjectGroup";
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

    AABB* bounding_box(){
      return new AABB(
          center - Vector(radius, radius, radius),
          center + Vector(radius, radius, radius)
      );
    }

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

    inline std::string str(std::string indent="") {
      return indent + "Sphere";
    }
};


class Plane: public Object {
  public:
    Vector point; // any point on the plane will do
    Vector norm;
    Material *material;

    Plane(Vector point, Vector norm, Material *material)
      : point(point), norm(norm), material(material) {}

    AABB* bounding_box(){
      return NULL;  // we could have an infinte bounding box. well..
    }

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

    inline std::string str(std::string indent="") {
      return indent + "Plane";
    }
};


#ifdef NO_MOLLER_TRUMBORE
// Standard intersection model

class Triangle: public Object {
  public:
    Vector a, b, c;
    Vector norm;
    Material *material;

    Vector A, B, C;

    Triangle(Vector a, Vector b, Vector c, Material *material)
        : a(a), b(b), c(c), material(material) {

      C = b - a;
      B = a - c;
      A = c - b;

      norm = -(C*B).unit();
    }

    AABB* bounding_box(){
      return new AABB(  // TODO replace min & max function with *args
          min(a, min(b, c)),
          max(a, max(b, c))
      );
    }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      nTriangleIntersection++;

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

        if(norm%(C*(hitpoint-a)) < 0) return NULL;
        if(norm%(A*(hitpoint-b)) < 0) return NULL;
        if(norm%(B*(hitpoint-c)) < 0) return NULL;

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

    inline std::string str(std::string indent="") {
      return indent + "Triangle";
    }
};


#else  // no NO_MOLLER_TRUMBORE

// Möller Trumbore intersection
// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

class Triangle: public Object {
  public:
    Vector a, b, c;  // TODO only storing b & c for the bounding box =|
    Material *material;

    Vector norm;
    Vector e1, e2;

    Triangle(Vector a, Vector b, Vector c, Material *material) : a(a), b(b), c(c), material(material) {
      e1 = b - a;
      e2 = c - a;
      norm = (e1*e2).unit();
    }

    AABB* bounding_box() {
      return new AABB(  // TODO replace min & max function with *args
          min(a, min(b, c)),
          max(a, max(b, c))
      );
    }

    virtual Vector normal(float u, float v){
      return norm;
    }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      nTriangleIntersection++;

      Vector P = ray.dir*e2;
      float det = e1%P;

      if (det < 0.0001f) return NULL;
      /* if (fabs(det) < kEpsilon) return NULL; */

      det = 1 / det;

      Vector T = ray.orig - a;
      float u = (T%P) * det;
      if (u < 0 || u > 1) return NULL;

      Vector Q = T*e1;
      float v = (ray.dir%Q) * det;
      if (v < 0 || u + v > 1) return NULL;

      float t = (e2%Q) * det;

      if(t > t_min && t < t_max)
        return new HitRecord(
            t,
            ray.point_at_parameter(t),
            normal(u, v),
            material,
            u,
            v
            );

      return NULL;
    }

    inline std::string str(std::string indent="") {
      return indent + "Triangle:\n"
             + indent + "  " + a.str() + ",\n"
             + indent + "  " + b.str() + ",\n"
             + indent + "  " + c.str() + "\n";
    }
};

#endif  // NO_MOLLER_TRUMBORE

class SmoothedTriangle : public Triangle {
  public:
    Vector na, nb, nc;

    SmoothedTriangle(Vector a, Vector b, Vector c, Vector na, Vector nb, Vector nc, Material *material) : Triangle(a, b, c, material), na(na), nb(nb), nc(nc) {
      e1 = b - a;
      e2 = c - a;
    }

    Vector normal(float u, float v){
      // Ray Tracing from the Ground Up p.479
      // u <=> beta, v <=> gamma
      return (
          (1-u-v) * na
          +  u    * nb
          +  v    * nc
        ).unit();
    }
};


class BHV : public Object {
  public:
    AABB box;

    Object* left;
    Object* right;

    BHV(std::vector<Object*> list, int depth=0) {
      int size = list.size();

      if(size < 2){
        std::cerr << "BHV init: should not have less than 2 objects" << std::endl;
        exit(1);
      }

      if(size == 2){
        left = list[0];
        right = list[1];
      } else {

        // sort around a random axis
        int axis = depth%3;
        /* int axis = int(3*RANDOM_FLOAT); */
        /* int axis = 2; */
        std::sort(list.begin(), list.end(), [axis] (Object* a, Object* b) {
          return (*(b->bounding_box())).vmin[axis] < (*(a->bounding_box())).vmin[axis];
        });

        if(size == 3)
          left = list[0];  // we want to avoid BHV node with only 1 object
        else
          left = new BHV(std::vector<Object*>(list.begin(), list.begin()+size/2), depth+1);
        right = new BHV(std::vector<Object*>(list.begin() + size/2, list.begin() + (2*size+1)/2), depth+1);
      }

      box = (*left->bounding_box()) & (*right->bounding_box());  // union of both bounding boxes
    }

    AABB *bounding_box() {
      return &box;
    }

    HitRecord *hit(Ray ray, float t_min, float t_max) {
      nBoxIntersection++;

      HitRecord* dummyRec = box.hit(ray, t_min, t_max); // TODO change by a bool returning function
      if(dummyRec != NULL){
        delete dummyRec;

        /* std::cout << "hit " << box.str() << " with " << ray.orig << " - dir " << ray.dir << std::endl; */

        HitRecord* rec_left = left->hit(ray, t_min, t_max);
        HitRecord* rec_right = right->hit(ray, t_min, t_max);

        if(rec_left != NULL){
          if(rec_right != NULL){
            if(rec_left->t < rec_right->t){
              delete rec_right;
              return rec_left;
            } else {
              delete rec_left;
              return rec_right;
            }
          }
          return rec_left;
        }

        if(rec_right != NULL)
          return rec_right;
      }

      return NULL;
    }

    inline std::string str(std::string indent="") {
      return indent + "BHV[\n" + box.str(indent+"==>")
             + left->str(indent+"  ") + ",\n"
             + right->str(indent+"  ") + "\n"
             + indent + "]\n";
    }

    inline int depth() {
      return std::max(left->depth(), right->depth()) + 1;
    }
};




// 'Fake primitive' - alternative constructors

inline ObjectGroup* box(Material *mat, Vector orig=Vector(-0.5f, 0.5f, -0.5f),
                        Vector size=VECTOR_ONE,
                        Vector rotation=VECTOR_ZERO  // in deg
                        ) {

  ObjectGroup* group = new ObjectGroup();

  //   /B---C
  //  A---D/|
  //  | F-|-G
  //  E/--H/

  // dirs

  Vector X = Vector(size.x, 0, 0);
  Vector Y = Vector(0, size.y, 0);
  Vector Z = Vector(0, 0, size.z);


  // apply rotations

  Vector rotRad = rotation*(M_PI/180.);
  X = X.rotate(rotRad);
  Y = Y.rotate(rotRad);
  Z = Z.rotate(rotRad);


  // vertices

  Vector A = orig;

  Vector B = A + Z;

  Vector C = B + X;
  Vector D = A + X;

  Vector E = A - Y;
  Vector F = B - Y;
  Vector G = C - Y;
  Vector H = D - Y;


  /* // norml debbuging */
  /* Vector center = A + (G-A)/2; */
  /* Vector NA = (A - center).unit(); */
  /* Vector NB = (B - center).unit(); */
  /* Vector NC = (C - center).unit(); */
  /* Vector ND = (D - center).unit(); */
  /* Vector NE = (E - center).unit(); */
  /* Vector NF = (F - center).unit(); */
  /* Vector NG = (G - center).unit(); */
  /* Vector NH = (H - center).unit(); */


  // walls

  // back
  group->add( new Triangle(F, G, B, mat) );
  group->add( new Triangle(C, B, G, mat) );

  // front
  group->add( new Triangle(E, A, H, mat) );
  group->add( new Triangle(D, H, A, mat) );


  // floor
  group->add( new Triangle(E, G, F, mat) );
  group->add( new Triangle(G, E, H, mat) );

  // ceiling
  group->add( new Triangle(A, B, C, mat) );
  group->add( new Triangle(C, D, A, mat) );


  // left
  group->add( new Triangle(A, E, B, mat) );
  group->add( new Triangle(B, E, F, mat) );

  // right
  group->add( new Triangle(D, C, H, mat) );
  group->add( new Triangle(C, G, H, mat) );

  return group;
};

#endif /* DEF_OBJECT */
