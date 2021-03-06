#ifndef DEF_OBJECT
#define DEF_OBJECT

#include <math.h>
#include <list>
#include <vector>
#include <algorithm>

#include "vector.h"
#include "stats.h"
#include "ray.h"

class Material;

struct HitRecord {
  HitRecord() {}
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

    virtual bool hit(const Ray& ray, HitRecord &res) const = 0;
    virtual AABB* bounding_box() = 0;

    virtual inline std::string str(std::string indent="") const {
      return indent + "<object>";
    }

    virtual inline int depth() const {
      return 1;
    }
};


// axis aligned bounding box
/* class AABB : public Object { */
class AABB {
  public:
    Vector vmin;
    Vector vmax;

    AABB() {
      vmin = VECTOR_MAX;
      vmax = VECTOR_MIN;
    };

    AABB(Vector vmin, Vector vmax): vmin(vmin), vmax(vmax) {}

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


    AABB* bounding_box() {
      return this;
    }

    inline Vector operator[] (int i) {
      return (i==0) ? vmin : vmax;
    }

    inline const Vector operator[] (int i) const {
      return (i==0) ? vmin : vmax;
    }

    inline const Vector getCenter() const {
      return (vmin+vmax)/2.f;
    }

    inline float surface() const {
      Vector size = vmax - vmin;
      return 2.f * (size.x*size.y
                  + size.y*size.z
                  + size.z*size.x);
    }

    bool hit(const Ray& ray, HitRecord &res) const {
      return hit(ray);
    }

    // easier to exec callgrind & such with another function without an hitrecord output
    bool hit(const Ray& ray) const {
      // http://www.cs.utah.edu/~awilliam/box/box.pdf
      const AABB &bounds = *this;

      float t1min = (bounds[ray.sign[0]].x - ray.orig.x) * ray.invdir.x;
      float t1max = (bounds[1-ray.sign[0]].x - ray.orig.x) * ray.invdir.x;
      float t2min = (bounds[ray.sign[1]].y - ray.orig.y) * ray.invdir.y;
      float t2max = (bounds[1-ray.sign[1]].y - ray.orig.y) * ray.invdir.y;

      if(t1min > t2max || t2min > t1max)
        return false;

      if (t2min > t1min)
        t1min = t2min;
      if (t2max < t1max)
        t1max = t2max;

      t2min = (bounds[ray.sign[2]].z - ray.orig.z) * ray.invdir.z;
      t2max = (bounds[1-ray.sign[2]].z - ray.orig.z) * ray.invdir.z;

      if(t1min > t2max || t2min > t1max)
        return false;

      if (t2min > t1min)
        t1min = t2min;
      if (t2max < t1max)
        t1max = t2max;

      return (t1min < ray.t_max && t1max > ray.t_min);
    }

    inline std::string str(std::string indent="") const {
      return indent + "AABB:\n"
              + indent + "  min: " + vmin.str()
              + indent + "  max: " + vmax.str();
    }
};

/* class AABBShape : public AABB, public Object { */
/*   AABBShape(Vector vmin, Vector vmax): AABB(vmin, vmax) {} */

/*   AABBShape* bounding_box() { */
/*     return this; */
/*   } */
/* }; */


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

    bool hit(const Ray& ray, HitRecord &res) const {
      bool bres = false;
      for(Object* obj : list)
        if(obj->hit(ray, res)){
          bres = true;
          ray.t_max = res.t;
        }
      return bres;
    }

    AABB* bounding_box() {
      return NULL;  // TODO? we dont really care atm
    }

    inline std::string str(std::string indent="") const {
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

    AABB* bounding_box() {
      return new AABB(
          center - Vector(radius, radius, radius),
          center + Vector(radius, radius, radius)
      );
    }

    bool hit(const Ray& ray, HitRecord &res) const {
      Vector oc = ray.orig - center;
      float a = ray.dir%ray.dir;
      float b = 2.0f * oc%ray.dir;
      float c = oc%oc - radius*radius;
      float discriminant = b*b-4.0f*a*c;

      float tmp;

      if(discriminant > 0.0f){
        float sqrtt = sqrt(discriminant);
        tmp = (-b-sqrtt)/(2.0f*a);

        if(tmp > ray.t_min && tmp < ray.t_max)
          goto returnHitRec;

        tmp = (-b+sqrtt)/(2.0f*a);
         /* tmp = -tmp - b/a;  // branch prediction makes this slower? why cant gcc optimize this.. */
        if(tmp > ray.t_min && tmp < ray.t_max)
          goto returnHitRec;
      }

      return false;


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

      ray.t_max = tmp;

      res.t = tmp;
      res.p = hitpoint;
      res.normal = norm;
      res.material = material;
      res.u = u;
      res.v = v;
      return true;
    }

    inline std::string str(std::string indent="") const {
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

    AABB* bounding_box() {
      return NULL;  // we could have an infinte bounding box. well..
    }

    bool hit(const Ray& ray, HitRecord &res) const {
      float ND = norm%ray.dir;
      if(abs(ND) < 0.00001f) // assume its parallel
        return false;

      float t = (norm%(point-ray.orig))/ND;
      if(t > ray.t_min && t < ray.t_max){
        ray.t_max = t;

        res.t = t;
        res.p = ray.point_at_parameter(t);
        res.normal = norm;
        res.material = material;
        res.u = 0.0f;
        res.v = 0.0f;
        return true;
      }

      return false;
    }

    inline std::string str(std::string indent="") const {
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

    AABB* bounding_box() {
      return new AABB(  // TODO replace min & max function with *args
          min(a, min(b, c)),
          max(a, max(b, c))
      );
    }

    bool hit(const Ray& ray, HitRecord &res) const {
      /* nTriangleIntersection++;  // @stats */

      float ND = norm%ray.dir;

      if(ND > -0.0001f)   // TODO change that if we want dielectric triangles
        return false;

      /* if(abs(ND) < 0.00001f) // assume its parallel */
      /*   return NULL; */

      float t = (norm%(a-ray.orig))/ND;
      if(t > ray.t_min && t < ray.t_max) {
        // check if its inside the triangle
        // we check that the point in on the left side of each directed edges
        Vector hitpoint = ray.point_at_parameter(t);

        if(norm%(C*(hitpoint-a)) < 0) return NULL;
        if(norm%(A*(hitpoint-b)) < 0) return NULL;
        if(norm%(B*(hitpoint-c)) < 0) return NULL;

        ray.t_max = t;

        res.t = t;
        res.p = hitpoint;
        res.normal = norm;
        res.material = material;
        res.u = 0.0f;
        res.v = 0.0f;
        return true;
      }

      return false;
    }

    inline std::string str(std::string indent="") const {
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

      // check normals
      /* for(int i=0 ; i<3 ; i++) */
      /*   if(isnan(norm[i]) || norm[i] < -1.1f) */
      /*     std::cout << "normal problem on triangle: " << str() << std::endl; */
    }

    AABB* bounding_box() {
      return new AABB(  // TODO replace min & max function with *args
          min(a, min(b, c)),
          max(a, max(b, c))
      );
    }

    virtual Vector normal(float u, float v) const {
      return norm;
    }

    bool hit(const Ray& ray, HitRecord &res) const {
      /* nTriangleIntersection++;  // @stats */

      Vector P = ray.dir*e2;
      float det = e1%P;

      if (det < 0.00000000001f) return false;
      /* if (fabs(det) < 0.0000000001f) return false; */

      det = 1 / det;

      Vector T = ray.orig - a;
      float u = (T%P) * det;
      if (u < 0 || u > 1) return false;

      Vector Q = T*e1;
      float v = (ray.dir%Q) * det;
      if (v < 0 || u + v > 1) return false;

      float t = (e2%Q) * det;

      if(t > ray.t_min && t < ray.t_max){
        ray.t_max = t;

        res.t = t;
        res.p = ray.point_at_parameter(t);
        res.normal = normal(u, v);
        res.material = material;
        res.u = u;
        res.v = v;
        return true;
      }

      return false;
    }

    inline std::string str(std::string indent="") const {
      return indent + "Triangle:\n"
             + indent + "  " + a.str() + ",\n"
             + indent + "  " + b.str() + ",\n"
             + indent + "  " + c.str() + ",\n"
             + indent + "  e1: " + e1.str() + ",\n"
             + indent + "  e2: " + e2.str() + ",\n"
             + indent + "  norm: " + norm.str() + "\n";
    }
};

#endif  // NO_MOLLER_TRUMBORE

class SmoothedTriangle : public Triangle {
  public:
    Vector na, nb, nc;

    SmoothedTriangle(Vector a, Vector b, Vector c, Material *material) : Triangle(a, b, c, material) {
      na = VECTOR_ONE;
      nb = VECTOR_ONE;
      nc = VECTOR_ONE;
    }

    SmoothedTriangle(Vector a, Vector b, Vector c, Vector na, Vector nb, Vector nc, Material *material) : Triangle(a, b, c, material), na(na), nb(nb), nc(nc) {}

    Vector normal(float u, float v) const {
      // Ray Tracing from the Ground Up p.479
      // u <=> beta, v <=> gamma

      /* return norm;  // unsmmothed triangles */
      return (
          (1-u-v) * na
          +  u    * nb
          +  v    * nc
        ).unit();
    }
};


struct _PrimitiveData {
  Object* primitive;
  AABB* bounding_box;
  Vector center;

  _PrimitiveData(Object* primitive): primitive(primitive) {
    bounding_box = primitive->bounding_box();
    center = bounding_box->getCenter();
  }
};


struct _BHVBuildNode {  // Linear storage
  AABB box;
  _BHVBuildNode* left;
  _BHVBuildNode* right;
  std::vector<_PrimitiveData*> primitiveData;

  int axis;

  _BHVBuildNode(AABB box, _BHVBuildNode* left, _BHVBuildNode* right, std::vector<_PrimitiveData*> primitiveData=std::vector<_PrimitiveData*>()):
    box(box), left(left), right(right), primitiveData(primitiveData) {}

  _BHVBuildNode(std::vector<_PrimitiveData*> list, int depth=0) {
    int size = list.size();

    /* std::cout << "bhvBuildNode init: size = " << size << std::endl; */

    primitiveData = std::vector<_PrimitiveData*>();

    if(size < 1){
      std::cerr << "BHV init: should not have less than 1 object" << std::endl;
      exit(1);
    }

    if(size <= 4){
      left = NULL;
      right = NULL;
      box = AABB();
      for(_PrimitiveData* pd: list) {
        primitiveData.push_back(pd);
        box = box & *pd->bounding_box;
      }

    } else {

#if BHV_NO_SAH
      axis = 0;
      float maxAxisDiff = -FLT_MAX;
      float midpoint = 0;
      for(int i=0 ; i<3 ; i++){
        float axisMin = FLT_MAX;
        float axisMax = -FLT_MAX;

        for(_PrimitiveData* primData : list){
#if 1    // center range
          if(primData->center[i] < axisMin)
            axisMin = primData->center[i];
          if(primData->center[i] > axisMax)
            axisMax = primData->center[i];

#else    // bounding boxes min/max
          if(primData->bounding_box->vmin[i] < axisMin)
            axisMin = primData->bounding_box->vmin[i];
          if(primData->bounding_box->vmax[i] > axisMax)
            axisMax = primData->bounding_box->vmax[i];
#endif
        }

        if(axisMax-axisMin > maxAxisDiff){
          maxAxisDiff = axisMax-axisMin;
          midpoint = (axisMin+axisMax)/2.f;
          axis = i;
        }
      }

      // sort around a random axis
      /* axis = depth%3; */
      /* int axis = int(3*RANDOM_FLOAT); */
      /* int axis = 2; */

#if 1 // equal count
      std::sort(list.begin(), list.end(), [this] (_PrimitiveData* a, _PrimitiveData* b) {
          return (*(b->bounding_box)).vmin[axis] < (*(a->bounding_box)).vmin[axis];
          /* return b->center[axis] > a->center[axis]; */
      });

      left = new _BHVBuildNode(std::vector<_PrimitiveData*>(list.begin(), list.begin()+size/2), depth+1);
      right = new _BHVBuildNode(std::vector<_PrimitiveData*>(list.begin() + size/2, list.begin() + (2*size+1)/2), depth+1);

#else  // compare to midpoint
      std::sort(list.begin(), list.end(), [this] (_PrimitiveData* a, _PrimitiveData* b) {
          return b->center[axis] > a->center[axis];
        });

      int midIdx = 0;
      while(list[midIdx]->center[axis] < midpoint)
        midIdx++;

      // all centers are the same point workaround
      if(midIdx == 0)
        midIdx++;

      /* std::cout << "midIdx: " << midIdx << ", size = " << size << ", midpoint: " << midpoint << ", min: " << list[0]->center[axis] << ", max: " << list[size-1]->center[axis] << std::endl; */

      left = new _BHVBuildNode(std::vector<_PrimitiveData*>(list.begin(), list.begin()+midIdx), depth+1);
      right = new _BHVBuildNode(std::vector<_PrimitiveData*>(list.begin() + midIdx, list.end()), depth+1);
#endif

      box = left->box & right->box;  // union of both bounding boxes

#else  // not BHV_NO_SAH

      axis = 0;  // best axis
      std::vector<_PrimitiveData*> bestLeftCut, bestRightCut;
      float minCost = FLT_MAX;  // min cost for best cut

      for(int axes=0 ; axes<3 ; axes++){

        // sort around current axis
        std::sort(list.begin(), list.end(), [axes] (_PrimitiveData* a, _PrimitiveData* b) {
          /* return (*(b->bounding_box)).vmin[axes] < (*(a->bounding_box)).vmin[axes]; */
          return b->center[axes] > a->center[axes];
        });

        // cut BEFORE i. so range from 1 to size-1

        AABB currentBox;
        float leftSurface[size];
        for(int i=1 ; i<size ; i++){
          currentBox = currentBox & *list[i-1]->bounding_box;
          leftSurface[i] = currentBox.surface();
        }

        int bestCutOffset = -1;

        currentBox = AABB();
        for(int i=size-1 ; i>=1 ; i--){
          currentBox = currentBox & *list[i]->bounding_box;
          float totalCost = i*leftSurface[i] + (size-i)*currentBox.surface();
          if(totalCost < minCost){
            axis = axes;
            minCost = totalCost;
            bestCutOffset = i;
          }
        }

        // i assume its ok to do max 4 avoidable vector allocation vs having to re-sort the list
        // => atm we keep the current best offset until the end of the inner axis loop, then compute both children
        // we could also only keep the offset & only compute the final children at the end, but we would need to sort again the list for the minimal costing axis
        // or we would be dumb (again) & do 2 alloc each time we reach a new minimum
        if(bestCutOffset != -1){
          bestLeftCut = std::vector<_PrimitiveData*>(list.begin(), list.begin()+bestCutOffset);
          bestRightCut = std::vector<_PrimitiveData*>(list.begin()+bestCutOffset, list.end());
        }
      }

      // minCost = n(left)*s(left) + n(right)*s(right)
      // cost = i(AABB) +  i(primitive)*minCost/s(total)
      // cost < n(total)*i(primitive) => split, else dont
      //
      // assume i(AABB) = 1/8 and i(primitive) = 1  TODO better


      left = new _BHVBuildNode(bestLeftCut);
      right = new _BHVBuildNode(bestRightCut);
      box = left->box & right->box;
      float cost = 1.f/8.f + 1.f * minCost/box.surface();

      if( cost >= size*1.f ){  // dont split
        left = NULL;
        right = NULL;
        box = AABB();
        for(_PrimitiveData* pd: list) {
          primitiveData.push_back(pd);
          box = box & *pd->bounding_box;
        }
      }  // else do split  -- already done at this point

#endif  // BHV_NO_SAH
    }
  }

  int nodeCount(){
    return primitiveData.size() > 0 ? primitiveData.size() : 1 + left->nodeCount() + right->nodeCount();
  }
};


struct BHVNode {
  AABB box;
  union {
    int rightNodeIdx;
    int primitiveIdx;
  };
  uint8_t nPrimitives;
  uint8_t axis;
};

/* enum BHVSplitMethod { */
/*   MIDDLE, */
/*   EQUAL_COUNT, */
/*   SAH, */
/* } */


class BHV {  // node
  public:
    BHVNode* nodes;
    std::vector<Object*> primitives;

    BHV(std::vector<Object*> list) {
      std::cout << "BHV construction: constructing primitive datas..." << std::endl;
      std::vector<_PrimitiveData*> data;
      data.reserve(list.size());
      for (std::size_t i = 0 ; i < list.size() ; i++)
        data.push_back(new _PrimitiveData(list[i]));

      std::cout << "BHV construction: first pass..." << std::endl;
      _BHVBuildNode* root = new _BHVBuildNode(data);

      std::cout << "BHV construction: packed structure allocation..." << std::endl;

      // pack every nodes
      // node array allocation
      int nodeCount = root->nodeCount();
      nodes = new BHVNode[nodeCount];
      for(int i=0 ; i<nodeCount ; i++)
        new (&nodes[i]) BHVNode;

      std::cout << "BHV construction: filling packaged structure... (node count: " << nodeCount << ")" << std::endl;
      // fill node array
      int offset = 0;
      packNodes(root, &offset);
      std::cout << "BHV construction: done. offset: " << offset << std::endl;
    }

    int packNodes(_BHVBuildNode* node, int *offset){
      int currentOffset = *offset;
      /* std::cout << "current offset: " << currentOffset << std::endl; */
      (*offset)++;

      BHVNode* compactNode = &nodes[currentOffset];
      compactNode->box = node->box;

      if(node->primitiveData.size() != 0) {
        compactNode->nPrimitives = node->primitiveData.size();
        compactNode->primitiveIdx = primitives.size();
        for(_PrimitiveData* pd: node->primitiveData)
          primitives.push_back(pd->primitive);

      } else {  // dfs
        compactNode->nPrimitives = 0;
        compactNode->axis = node->axis;
        /* if(node->axis > 2 || node ->axis < 0) exit(1); */
        packNodes(node->left, offset);
        compactNode->rightNodeIdx = packNodes(node->right, offset);
      }

      return currentOffset;
    }

    AABB *bounding_box() {
      return NULL;
    }

    AABB *bounding_box() const {
      return &nodes[0].box;  // root's bounding box
    }

    bool hit(const Ray& ray, HitRecord &res) const {
      bool hit = false;

      int stack[50];  // node position in 'nodes' array
      int stackPos = 0;
      int nodeIdx = 0;  // 1st one is the root

      while(true) {
        /* nBoxIntersection++;  // @stats */

        const BHVNode* currentNode = &nodes[nodeIdx];
        if(currentNode->box.hit(ray)){
          /* std::cout << "hit " << box.str() << " with " << ray.orig << " - dir " << ray.dir << std::endl; */

          if(currentNode->nPrimitives != 0) {
            for(int iprim=0 ; iprim<currentNode->nPrimitives ; iprim++)
              if(primitives[currentNode->primitiveIdx+iprim]->hit(ray, res))
                hit = true;

            if(stackPos == 0)
              break;
            stackPos--;
            nodeIdx = stack[stackPos];

          } else {
            if(!ray.sign[currentNode->axis]) {
              stack[stackPos] = nodeIdx+1;
              stackPos++;
              nodeIdx = currentNode->rightNodeIdx;
            } else {
              stack[stackPos] = currentNode->rightNodeIdx;
              stackPos++;
              nodeIdx++;
            }
          }

        } else {
          if(stackPos == 0)
            break;
          stackPos--;
          nodeIdx = stack[stackPos];
        }
      }

      return hit;
    }


    int traversal_count(const Ray& ray) const {
      int stack[50];  // node position in 'nodes' array
      int stackPos = 0;
      int nodeIdx = 0;  // 1st one is the root

      int count = 0;

      while(true) {
        count++;

        const BHVNode* currentNode = &nodes[nodeIdx];
        if(currentNode->box.hit(ray)){
          if(currentNode->nPrimitives != 0) {
            /* count += currentNode->nPrimitives; */

            if(stackPos == 0)
              break;
            stackPos--;
            nodeIdx = stack[stackPos];

          } else {
            if(!ray.sign[currentNode->axis]) {
              stack[stackPos] = nodeIdx+1;
              stackPos++;
              nodeIdx = currentNode->rightNodeIdx;
            } else {
              stack[stackPos] = currentNode->rightNodeIdx;
              stackPos++;
              nodeIdx++;
            }
          }

        } else {
          if(stackPos == 0)
            break;
          stackPos--;
          nodeIdx = stack[stackPos];
        }
      }

      return count;
    }

    /* inline std::string str(std::string indent="") const { */
    /*   return indent + "BHV[\n" + box.str(indent+"==>") */
    /*          + left->str(indent+"  ") + ",\n" */
    /*          + right->str(indent+"  ") + "\n" */
    /*          + indent + "]\n"; */
    /* } */

    /* inline int depth() const { */
    /*   return std::max(left->depth(), right->depth()) + 1; */
    /* } */
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
