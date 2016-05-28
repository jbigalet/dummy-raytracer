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

  /* int width = 400; */
  /* int height = 200; */

  /* int width = 1600; */
  /* int height = 800; */

  /* int width = 1920/2; */
  /* int height = 1080/2; */

  /* int width = 1200; */
  /* int height = 600; */

  int width = 600;
  int height = 300;

  int nsamples = 50;
  /* int nsamples = 100; */
  /* int nsamples = 200; */
  /* int nsamples = 1000; */

  int max_bounces = 100;

  std::ofstream image;
  image.open("image.ppm");
  /* image.open("image_"+std::to_string(nsamples)+".ppm"); */

  image << "P3\n" << width << " " << height << "\n255\n";

  // above on the left
  /* Camera camera( */
  /*     width, */
  /*     height, */
  /*     Vector(-2, 2, 1), */
  /*     Vector(0, 0, -1), */
  /*     Vector(0, 1, 0), */
  /*     90 */
  /* ); */

  // in front
  Camera camera(
      width,
      height,
      Vector(0, 0, 0.5),
      Vector(0, 0, -1),
      Vector(0, 1, 0),
      90
  );

  ObjectGroup *world = new ObjectGroup();

  // 2 metal + 1 lamb in the middle
  /* world->add( new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(Vector(0.8, 0.8, 0))) ); */
  /* world->add( new Sphere(Vector(0, 0, -1), 0.5, new Lambertian(Vector(0.8, 0.3, 0.3))) ); */
  /* world->add( new Sphere(Vector(1, 0, -1), 0.5, new Metal(Vector(0.8, 0.6, 0.2), 0)) ); */
  /* world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Metal(Vector(0.8, 0.8, 0.8), 0.1)) ); */

  world->add( new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(Vector(0.8, 0.8, 0))) );
  world->add( new Sphere(Vector(0, 0, -1), 0.5, new Lambertian(Vector(0.8, 0.3, 0.3))) );
  world->add( new Sphere(Vector(1, 0, -1), 0.5, new Metal(Vector(0.9, 0.9, 0.9), 0)) );
  world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Dielectric(1.5f)) );
  world->add( new Sphere(Vector(-1, 0, -1), 0.45, new Dielectric(1.f, 1.5f)) );

  for(int j=height-1 ; j>=0; j--)
    for(int i=0 ; i<width ; i++) {

      Vector color = camera.getColor(*world, i, j, max_bounces, nsamples);

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
