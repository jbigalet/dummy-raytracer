#ifndef DEF_VECTOR
#define DEF_VECTOR

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "utils.h"

#define VECTOR_ZERO Vector(0,0,0)
#define VECTOR_ONE Vector(1,1,1)

#define VECTOR_MIN Vector(-FLT_MAX, -FLT_MAX, -FLT_MAX)
#define VECTOR_MAX Vector(FLT_MAX, FLT_MAX, FLT_MAX)

class Vector {
  public:
    float x, y, z;


    Vector() {
      x = 0.f;
      y = 0.f;
      z = 0.f;
    };
    Vector(float a, float b, float c): x(a), y(b), z(c) {}

    // clone constructor
    /* Vector(const Vector &v) { */
    /*   x = v.x; */
    /*   y = v.y; */
    /*   z = v.z; */
    /* } */

    ~Vector() {};

    inline float length() const { return sqrt(x*x+y*y+z*z); }

    inline Vector unit() const;

    // rotation - by the angles defined in the vector arg
    inline Vector rotate(const Vector &v) const {

      // around x
      float cosx = cos(v.x);
      float sinx = sin(v.x);
      float ny = y * cosx - z * sinx;
      float nz = z * cosx + y * sinx;

      // around y
      float cosy = cos(v.y);
      float siny = sin(v.y);
      float oldz = nz;
      float nx = x * cosy + nz * siny;
      nz = oldz * cosy - x * siny;

      // around z
      float cosz = cos(v.z);
      float sinz = sin(v.z);
      float oldx = nx;
      float oldy = ny;
      nx = oldx * cosz - oldy * sinz;
      ny = oldy * cosz + oldx * sinz;

      return Vector(nx, ny, nz);
    }

    inline int* toRGB(float gamma=1.0) const {
      return new int[3] {
        int(255*pow(x, 1.0/gamma)),
        int(255*pow(y, 1.0/gamma)),
        int(255*pow(z, 1.0/gamma))
      };
    }

    // max out all floats to 1
    inline void normalize_color() {
      if(x > 1.f) x = 1.f;
      if(y > 1.f) y = 1.f;
      if(z > 1.f) z = 1.f;
    }

    inline Vector tone_map() const {
      return Vector(
          x/(x+1),
          y/(y+1),
          z/(z+1)
      );
    }

    inline std::string str() const {
      return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]";
    }

// Operators

    inline Vector operator+() const { return *this; }
    inline Vector operator-() const { return Vector(-x, -y, -z); }

    inline float operator[] (int i) const {
      if(i==0) return x;
      if(i==1) return y;
      if(i==2) return z;

      // should not happen - to avoid Wall bitching about it
      std::cerr << "trying to access vector at index " << i << std::endl;
      exit(1);
    }

    inline float & operator[] (int i) {
      if(i==0) return x;
      if(i==1) return y;
      if(i==2) return z;

      // should not happen - to avoid Wall bitching about it
      std::cerr << "trying to access vector at index " << i << std::endl;
      exit(1);
    }

    inline bool operator==(const Vector &v) const {
      return x == v.x
          && y == v.y
          && z == v.z;
    }


// Assignment operators

    inline Vector operator+=(const Vector &v2) {
      x += v2.x;
      y += v2.y;
      z += v2.z;
      return *this;
    }

    inline Vector operator-=(const Vector &v2) {
      x -= v2.x;
      y -= v2.y;
      z -= v2.z;
      return *this;
    }

    inline Vector operator*=(const float f) {
      x *= f;
      y *= f;
      z *= f;
      return *this;
    }

    inline Vector operator/=(const float f) {
      x /= f;
      y /= f;
      z /= f;
      return *this;
    }

};


// Operators

inline Vector operator+(const Vector &v1, const Vector &v2) {
  return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector operator-(const Vector &v1, const Vector &v2) {
  return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector operator*(const float f, const Vector &v) {
  return Vector(v.x * f, v.y * f, v.z * f);
}
inline Vector operator*(const Vector &v, const float f) { return f*v; }

inline Vector operator/(const Vector &v, const float f) {
  return Vector(v.x / f, v.y / f, v.z / f);
}


// Special operators

// dot product
inline float operator%(const Vector &v1, const Vector &v2) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

// cross product
inline Vector operator*(const Vector &v1, const Vector &v2) {
  return Vector(
      v1.y*v2.z - v1.z*v2.y,
      -v1.x*v2.z + v1.z*v2.x,
      v1.x*v2.y - v1.y*v2.x
  );
}

// element by element product (for color computation)
inline Vector operator^(const Vector &v1, const Vector &v2) {
  return Vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

// reflection
inline Vector operator|(const Vector &v, const Vector &norm) {
  return v - 2.f*(v%norm) * norm;
}



// refraction
// from http://www.flipcode.com/archives/reflection_transmission.pdf
// iratio = refraction indice from / refraction indice to
// returns to &refracted
inline Vector* refract(const Vector &v, const Vector &norm, float iratio) {
  float cosI = v%norm;
  float sinT2 = iratio * iratio * (1.f - cosI*cosI);
  if(sinT2 > 1.f)
    return NULL;

  /* return new Vector(iratio*v - (iratio + sqrt(1.f-sinT2)) * norm); */
  return new Vector(iratio*(v-norm*cosI) - norm*sqrt(1.f - sinT2));
}

inline Vector min(const Vector &v1, const Vector &v2){
  return Vector(
      fmin(v1.x, v2.x),
      fmin(v1.y, v2.y),
      fmin(v1.z, v2.z)
  );
}

inline Vector max(const Vector &v1, const Vector &v2){
  return Vector(
      fmax(v1.x, v2.x),
      fmax(v1.y, v2.y),
      fmax(v1.z, v2.z)
  );
}

inline Vector Vector::unit() const { return (*this)/length(); }


inline Vector random_point_in_sphere() {
  Vector random_point;

  do {

    random_point = Vector(
       random_float_minus_one(),
       random_float_minus_one(),
       random_float_minus_one()
    );

  } while (random_point%random_point >= 1.0);

  return random_point;


  // alternate method: http://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability
  // a bit slower =[

  /* float delta = random_float(); */
  /* float u = random_float_minus_one(); */
  /* float theta = random_float_2pi(); */

  /* float delta3 = pow(delta, 1.0f/3.0f); */
  /* float u2 = u*u; */
  /* float udist = sqrt(1.0f-u2); */

  /* return Vector(delta3*udist*cos(theta), */
  /*               delta3*udist*sin(theta), */
  /*               delta3*u); */
}


// to string
inline std::ostream& operator<<(std::ostream &os, Vector const &v){
  return os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

#endif /* DEF_VECTOR */
