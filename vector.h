#ifndef DEF_VECTOR
#define DEF_VECTOR

#include <math.h>
#include <stdlib.h>

#include "utils.h"

#define VECTOR_ZERO Vector(0,0,0)
#define VECTOR_ONE Vector(1,1,1)

class Vector {
  public:
    float x, y, z;


    Vector() {};
    Vector(float a, float b, float c): x(a), y(b), z(c) {}

    ~Vector() {};

    inline float length() const { return sqrt(x*x+y*y+z*z); }

    inline Vector unit() const;

    inline int* toRGB(float gamma=1.0){
      return new int[3] {
        int(255*pow(x, 1.0/gamma)),
        int(255*pow(y, 1.0/gamma)),
        int(255*pow(z, 1.0/gamma))
      };
    }


// Operators

    inline Vector operator+() const { return *this; }
    inline Vector operator-() const { return Vector(-x, -y, -z); }


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
inline bool refract(const Vector &v, const Vector &norm, float iratio, Vector& refracted) {
  float cosI = v%norm;
  float sinT2 = iratio * iratio * (1.f - cosI*cosI);
  if(sinT2 > 1.f)
    return false;

  refracted  = iratio*v - (iratio + sqrt(1.f-sinT2)) * norm;
  return true;
}


inline Vector Vector::unit() const { return (*this)/length(); }


inline Vector random_point_in_sphere() {
  Vector random_point;

  do {

    random_point = Vector(
        RANDOM_FLOAT,
        RANDOM_FLOAT,
        RANDOM_FLOAT
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

#endif /* DEF_VECTOR */
