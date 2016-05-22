#include <iostream>

#include "vector.h"
#include "ray.h"
#include "object.h"
#include "shapes/sphere.h"

int main() {
  int width = 200;
  int height = 100;

  std::cout << "P3\n" << width << " " << height << "\n255\n";

  Vector lower_left_corner(-2, -1, -1);
  Vector horizontal(4, 0, 0);
  Vector vertical(0, 2, 0);
  Vector origin = VECTOR_ZERO;


  for(int j=height-1 ; j>=0; j--)
    for(int i=0 ; i<width ; i++) {

      float u = float(i)/float(width);
      float v = float(j)/float(height);

      Ray r(origin, lower_left_corner + u*horizontal + v*vertical);

      Sphere sphere(Vector(0, 0, -1), 0.5);

      Vector color = r.color(sphere, 100);
      int *rgb = color.toRGB();

      std::cout << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
    }
}
