#ifndef DEF_VECTOR
#define DEF_VECTOR

#include <math.h>

class Vector {
  public:
    float x, y, z;


    Vector() {};
    Vector(float a, float b, float c): x(a), y(b), z(c) {}

    ~Vector();

    inline float length() const { return sqrt(x*x+y*y+z*z); }


// Operators

    inline Vector operator+() const { return *this; }
    inline Vector operator-() const { return Vector(-x, -y, -z); }

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

    inline Vector operator/(const float f, const Vector &v) {
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
         v1.x*v2.y - v1.y*v2.x,
      );
    }


// Assignment operators

    inline Vector operator+=(const Vector &v2) const {
      x += v2.x;
      y += v2.y;
      z += v2.z;
      return *this;
    }

    inline Vector operator-=(const Vector &v2) const {
      x -= v2.x;
      y -= v2.y;
      z -= v2.z;
      return *this;
    }

    inline Vector operator*=(const float f) const {
      x *= f;
      y *= f;
      z *= f;
      return *this;
    }

    inline Vector operator/=(const float f) const {
      x /= f;
      y /= f;
      z /= f;
      return *this;
    }

};

#endif /* DEF_VECTOR */