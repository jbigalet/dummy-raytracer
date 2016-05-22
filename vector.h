#ifndef DEF_VECTOR
#define DEF_VECTOR

#include <math.h>
#include <stdlib.h>

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

inline Vector Vector::unit() const { return (*this)/length(); }


Vector random_point_in_sphere() {
  Vector random_point;

  do {

    random_point = Vector(
        drand48(),
        drand48(),
        drand48()
        );

  } while (random_point%random_point >= 1.0);

  return random_point;
}

#endif /* DEF_VECTOR */
