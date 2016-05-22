#include <iostream>

#include "vector.h"
#include "material.h"
#include "ray.h"
#include "object.h"
#include "camera.h"

int main() {
  int width = 200;
  int height = 100;

  std::cout << "P3\n" << width << " " << height << "\n255\n";

  Camera camera(
      width,
      height,
      Vector(0, 0, 1),
      Vector(-2, -1, -1),
      Vector(4, 0, 0),
      Vector(0, 2, 0)
  );

  /* Sphere *sphere = new Sphere(Vector(0, 0, -1), 0.5, new Lambertian(Vector(0.8, 0.8, 0))); */
  Sphere *sphere = new Sphere(Vector(0, -1000.5, -1), 1000, new Lambertian(Vector(0.8, 0.8, 0)));

  for(int j=height-1 ; j>=0; j--)
    for(int i=0 ; i<width ; i++) {

      Vector color = camera.getColor(*sphere, i, j, 2, 100);

      int *rgb = color.toRGB();
      std::cout << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
    }
}
