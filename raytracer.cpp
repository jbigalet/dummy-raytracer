#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "vector.h"
#include "material.h"
#include "ray.h"
#include "object.h"
#include "camera.h"
#include "texture.h"
#include "stats.h"

void rt_pass(Camera camera, ObjectGroup world, int width, int height, int max_bounces, int nsamples, Vector** out) {
  for(int i=0 ; i<width ; i++)
    for(int j=0 ; j<height ; j++)
      out[i][j] = camera.getColor(world, i, j, max_bounces, nsamples);
}

int main() {
  auto startTime = std::chrono::high_resolution_clock::now();

  int nthreads = 4;

  /* int width = 400; */
  /* int height = 200; */

  /* int width = 1600; */
  /* int height = 800; */

  int width = 1920/4;
  int height = 1080/4;

  /* int width = 1920/2; */
  /* int height = 1080/2; */

  /* int width = 1920; */
  /* int height = 1080; */

  /* int width = 1200; */
  /* int height = 600; */

  /* int width = 600; */
  /* int height = 300; */

  /* int nsamples = 10; */
  /* int nsamples = 20; */
  /* int nsamples = 50; */
  /* int nsamples = 100; */
  /* int nsamples = 200; */
  /* int nsamples = 500; */
  /* int nsamples = 1000; */
  int nsamples = 2000;
  /* int nsamples = 5000; */

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
      Vector(-1.8, 1., -2),
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

  // bubble-lamb-metal
  //
  world->add( new Plane(Vector(0, -0.5f, 0), Vector(0.f, 1.f, 0.f), new Lambertian(new ConstantTexture(0.8, 0.8, 0))) );
  /* world->add( new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(new ConstantTexture(0.8, 0.8, 0))) ); */

  /* world->add( new Sphere(Vector(0, 0, -1), */
  /*                        0.5, */
  /*                        new Lambertian( */
  /*                          new CheckerTexture( */
  /*                            new ConstantTexture(0.f, 0.f, 0.f), */
  /*                            new ConstantTexture(1.f, 1.f, 1.f))))); */

  world->add( new Sphere(Vector(0, 0, -1),
        0.5,
        new Lambertian(
          new ImageTexture("earth.bmp"))));

  world->add( new Triangle(Vector(0, 1.5f, -1), Vector(0, 0.5f, -1), Vector(0, 0.5f, -2), new Lambertian(new ConstantTexture(0.8f, 0.f, 0.f))) );

  world->add( new Sphere(Vector(-0.5, 0, -2.2), 0.5, new Metal(new ConstantTexture(0.9, 0.9, 0.9), 0)) );
  world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Dielectric(1.5f)) );
  world->add( new Sphere(Vector(-1, 0, -1), 0.45, new Dielectric(1.f, 1.5f)) );
  world->add( new Sphere(Vector(-2, 0, -1), 0.5, new Light(new ConstantTexture(20.f, 20.f, 20.f))) );
  world->add( new Sphere(Vector(0, 2, -1.5), 0.5, new Light(new ConstantTexture(20.f, 20.f, 20.f))) );


  // start job threads
  std::thread *threads = new std::thread[nthreads];
  Vector*** out = new Vector**[nthreads];
  for(int ithread=0 ; ithread<nthreads ; ithread++) {
    out[ithread] = new Vector*[width];
    for(int i=0 ; i<width ; i++)
      out[ithread][i] = new Vector[height];

    threads[ithread] = std::thread(rt_pass, camera, *world, width, height, max_bounces, nsamples/nthreads, out[ithread]);
  }


  // join threads
  for(int ithread=0 ; ithread<nthreads ; ithread++)
    threads[ithread].join();


  // average res & write the image
  for(int j=height-1 ; j>=0; j--)
    for(int i=0 ; i<width ; i++) {
      Vector average = VECTOR_ZERO;
      for(int ithread=0 ; ithread<nthreads ; ithread++)
        average += out[ithread][i][j];
      average = average/nthreads;
      int *rgb = average.toRGB(2);
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
