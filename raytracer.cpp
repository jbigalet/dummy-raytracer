#include <iostream>
#include <fstream>
#include <chrono>

#include "vector.h"
#include "camera.h"
#include "stats.h"
#include "utils.h"

int main() {
  auto startTime = std::chrono::high_resolution_clock::now();

  /* int width = 1600; */
  /* int height = 800; */
  int width = 400;
  int height = 200;

  std::ofstream image;
  image.open("image.ppm");

  image << "P3\n" << width << " " << height << "\n255\n";

  Camera camera(
      width,
      height,
      Vector(-2, 2, 1),
      Vector(0, 0, -1),
      Vector(0, 1, 0),
      90
  );

  Vector orig = camera.orig;
  Vector lower_left_corner = camera.lower_left_corner;
  Vector hor = camera.hor;
  Vector ver = camera.ver;

  /* ObjectGroup *world = new ObjectGroup(); */

  /* float x = 0.0f; */
  /* float y = -100.5f; */
  /* float z = -1.0f; */
  Vector sphere_center = Vector(0.0f, -100.5f, -1.0f);
  float sphere_radius = 100.0f;
  Vector sphere_att(0.8f, 0.8f, 0.0f);

  /* Sphere *world = new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(Vector(0.8, 0.8, 0))); */
  /* world->add( new Sphere(Vector(0, 0, -1), 0.5, new Lambertian(Vector(0.8, 0.3, 0.3))) ); */
  /* world->add( new Sphere(Vector(1, 0, -1), 0.5, new Metal(Vector(0.8, 0.6, 0.2), 0)) ); */
  /* world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Metal(Vector(0.8, 0.8, 0.8), 0.1)) ); */

  for(int j=height-1 ; j>=0; j--)
    for(int i=0 ; i<width ; i++) {

      // TODO samples
      int bounce = 100;

      Vector ray_orig = orig;
      Vector ray_dir = lower_left_corner + hor*((i+RANDOM_FLOAT)/width) + ver*((j+RANDOM_FLOAT)/height) - orig;
      Vector ray_att = VECTOR_ONE;




      Vector oc = ray_orig - sphere_center;
      float a = ray_dir%ray_dir;
      float b = 2.0f * oc%ray_dir;
      float c = oc%oc - sphere_radius*sphere_radius;
      float discriminant = b*b-4.0f*a*c;

      float hit_time = 0.0f;
      bool did_hit = true;

      if(discriminant > 0.0f) {
        float hit_time = (-b-sqrt(discriminant))/(2.0f*a);
        /* if(hit_time < t_min || hit_time > t_max){ */
        if(hit_time < 0.001f) {
          hit_time = (-b+sqrt(discriminant))/(2.0f*a);
          /* if(hit_time < t_min || hit_time > t_max) */
          if(hit_time < 0.001f)
            did_hit = false;
        }
      } else {
        did_hit = false;
      }

      Vector color;

      if(did_hit){
        /* ray_point = ray_orig + tmp*ray_dir; */
        /* return new HitRecord( */
        /*     tmp, */
        /*     ray_point, */
        /*     (ray_point - sphere_center) / sphere_radius */
        /*     ); */

        color = VECTOR_ONE;

      } else {
        float t = 0.5f*(ray_dir.unit().y + 1.0f);
        color = (1.0-t)*VECTOR_ONE + t*Vector(0.5f, 0.7f, 1.0f);
      }


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
