#include <iostream>

#include "vector.h"
#include "material.h"
#include "ray.h"
#include "object.h"
#include "camera.h"

int main() {
  int width = 400;
  int height = 200;

  std::cout << "P3\n" << width << " " << height << "\n255\n";

  Camera camera(
      width,
      height,
      Vector(0, 0, 1),
      Vector(-2, -1, -1),
      Vector(4, 0, 0),
      Vector(0, 2, 0)
  );

  ObjectGroup *world = new ObjectGroup();

  world->add( new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(Vector(0.8, 0.8, 0))) );
  world->add( new Sphere(Vector(0, 0, -1), 0.5, new Lambertian(Vector(0.8, 0.3, 0.3))) );
  world->add( new Sphere(Vector(1, 0, -1), 0.5, new Metal(Vector(0.8, 0.6, 0.2), 0)) );
  world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Metal(Vector(0.8, 0.8, 0.8), 0)) );

  for(int j=height-1 ; j>=0; j--)
    for(int i=0 ; i<width ; i++) {

      Vector color = camera.getColor(*world, i, j, 100, 100);

      int *rgb = color.toRGB(2);
      std::cout << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
    }
}
