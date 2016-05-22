#include <iostream>
#include <fstream>
#include <chrono>

#include "vector.h"
#include "material.h"
#include "ray.h"
#include "object.h"
#include "camera.h"
#include "stats.h"

int main() {
  auto startTime = std::chrono::high_resolution_clock::now();

  int width = 200;
  int height = 100;

  std::ofstream image;
  image.open("image.ppm");

  image << "P3\n" << width << " " << height << "\n255\n";

  Camera camera(
      width,
      height,
      Vector(0, 0.5, 0),
      Vector(0, 0, -1),
      Vector(0, 1, 0),
      90
  );

  ObjectGroup *world = new ObjectGroup();

  world->add( new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(Vector(0.8, 0.8, 0))) );
  world->add( new Sphere(Vector(0, 0, -1), 0.5, new Lambertian(Vector(0.8, 0.3, 0.3))) );
  world->add( new Sphere(Vector(1, 0, -1), 0.5, new Metal(Vector(0.8, 0.6, 0.2), 0)) );
  world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Metal(Vector(0.8, 0.8, 0.8), 0.1)) );

  for(int j=height-1 ; j>=0; j--)
    for(int i=0 ; i<width ; i++) {

      Vector color = camera.getColor(*world, i, j, 100, 100);

      int *rgb = color.toRGB(2);
      image << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
    }

  image.close();

  auto endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> totalTime = endTime - startTime;
  std::cout << "\nTotal: " << totalTime.count() << " ms\n";
  std::cout << "\nDirect rays: " << nDirectRay;
  std::cout << "\nTotal rays: " << nTotalRay << "\n";
  std::cout << "\nRay per seconds: " << nTotalRay/totalTime.count() << " k\n";

  return 0;
}
