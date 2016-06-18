#include <iostream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <thread>

#include "stats.h"
#include "vector.h"

#include "material.h"
#include "ray.h"
#include "object.h"
#include "camera.h"
#include "texture.h"

void rt_pass(Camera camera, Object* world, int width, int height, int max_bounces, int nsamples, Vector** out) {
  for(int i=0 ; i<width ; i++){
    /* std::cout << i << std::endl; */
    for(int j=0 ; j<height ; j++)
      out[i][j] = camera.getColor(*world, i, j, max_bounces, nsamples);
  }
}

int main() {
  auto startTime = std::chrono::high_resolution_clock::now();

  int nthreads = 1;
  /* int nthreads = 2; */
  /* int nthreads = 4; */

  /* int width = 400; */
  /* int height = 200; */

  /* int width = 1600; */
  /* int height = 800; */

  /* int width = 1920/8; */
  /* int height = 1080/8; */

  /* int width = 1920/4; */
  int height = 1080/4;
  /* int height = 50; */

  /* int width = 1920/2; */
  /* int height = 1080/2; */

  /* int width = 1920; */
  /* int height = 1080; */

  /* int width = 1200; */
  /* int height = 600; */

  /* int width = 600; */
  /* int height = 300; */

  int width = height;

  /* int nsamples = 1; */
  int nsamples = 4;
  /* int nsamples = 20; */
  /* int nsamples = 50; */
  /* int nsamples = 100; */
  /* int nsamples = 200; */
  /* int nsamples = 500; */
  /* int nsamples = 1000; */
  /* int nsamples = 2000; */
  /* int nsamples = 5000; */
  /* int nsamples = 10000; */

  /* int max_bounces = 2; */
  int max_bounces = 5;
  /* int max_bounces = 10; */
  /* int max_bounces = 100; */

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
  /* Camera camera( */
  /*     width, */
  /*     height, */
  /*     Vector(-1.8, 1., -2), */
  /*     Vector(0, 0, -1), */
  /*     Vector(0, 1, 0), */
  /*     90 */
  /* ); */

  // inside the box, looking in front
  Camera camera(
      width,
      height,
      /* Vector(0, 0, -3.f),  // cow */
      /* Vector(-0.5f, 1.f, -6.f),  // bunny */
      Vector(0, 0, -3.8f),  // cornell box
      /* Vector(0, 0, -1.1f), */
      /* Vector(-0.5f, 1.f, 0),  // bunny pos */
      Vector(0.f, 0.f, 0.f),
      Vector(0, 1, 0),
      40
      );

  ObjectGroup *world = new ObjectGroup();

  // 2 metal + 1 lamb in the middle
  /* world->add( new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(Vector(0.8, 0.8, 0))) ); */
  /* world->add( new Sphere(Vector(0, 0, -1), 0.5, new Lambertian(Vector(0.8, 0.3, 0.3))) ); */
  /* world->add( new Sphere(Vector(1, 0, -1), 0.5, new Metal(Vector(0.8, 0.6, 0.2), 0)) ); */
  /* world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Metal(Vector(0.8, 0.8, 0.8), 0.1)) ); */

  // bubble-lamb-metal
  //
  /* world->add( new Plane(Vector(0, -0.5f, 0), Vector(0.f, 1.f, 0.f), new Lambertian(new ConstantTexture(0.8, 0.8, 0))) ); */
  /* world->add( new Sphere(Vector(0, -100.5, -1), 100, new Lambertian(new ConstantTexture(0.8, 0.8, 0))) ); */

  /* world->add( new Sphere(Vector(0, 0, -1), */
  /*                        0.5, */
  /*                        new Lambertian( */
  /*                          new CheckerTexture( */
  /*                            new ConstantTexture(0.f, 0.f, 0.f), */
  /*                            new ConstantTexture(1.f, 1.f, 1.f))))); */

  /* world->add( new Sphere(Vector(0, 0, -1), */
  /*       0.5, */
  /*       new Lambertian( */
  /*         new ImageTexture("earth.bmp")))); */

  /* world->add( new Triangle(Vector(0.2f, 0.5f, -1), Vector(0, 1.5f, -1), Vector(0, 0.5f, -2), new Metal(new ConstantTexture(0.8f, 0.8f, 0.8f), 0.0f)) ); */

  /* world->add( new Sphere(Vector(-0.5, 0, -2.2), 0.5, new Metal(new ConstantTexture(0.9, 0.9, 0.9), 0)) ); */
  /* world->add( new Sphere(Vector(-1, 0, -1), 0.5, new Dielectric(1.5f)) ); */
  /* world->add( new Sphere(Vector(-1, 0, -1), 0.45, new Dielectric(1.f, 1.5f)) ); */
  /* world->add( new Sphere(Vector(-2, 0, -1), 0.5, new Light(new ConstantTexture(20.f, 20.f, 20.f))) ); */
  /* world->add( new Sphere(Vector(0, 2, -1.5), 0.5, new Light(new ConstantTexture(20.f, 20.f, 20.f))) ); */


  // cornell box
  //
  //   /B---C
  //  A---D/|
  //  | F-|-G
  //  E/--H/

  Vector A = Vector(-1, 1, -1);
  Vector B = Vector(-1, 1, 1);
  Vector C = Vector(1, 1, 1);
  Vector D = Vector(1, 1, -1);
  Vector E = Vector(-1, -1, -1);
  Vector F = Vector(-1, -1, 1);
  Vector G = Vector(1, -1, 1);
  Vector H = Vector(1, -1, -1);

  // back
  Material* backMat = new Lambertian(new ConstantTexture(1.f, 1.f, 1.f));
  world->add( new Triangle(F, B, G, backMat));
  world->add( new Triangle(C, G, B, backMat));

  // front - letting ray in from the camera
  // /!\ not part of the original cornell box
  /* Material* frontMat = new Lambertian(new ConstantTexture(1.f, 1.f, 1.f)); */
  /* world->add( new Triangle(E, H, A, frontMat)); */
  /* world->add( new Triangle(D, A, H, frontMat)); */

  // floor
  Material* floorkMat = new Lambertian(new ConstantTexture(1.f, 1.f, 1.f));
  world->add( new Triangle(E, F, G, floorkMat));
  world->add( new Triangle(G, H, E, floorkMat));

  // ceiling
  Material* ceilingMat = new Lambertian(new ConstantTexture(1.f, 1.f, 1.f));
  /* Material* ceilingMat = new Metal(new ConstantTexture(1.f, 1.f, 1.f), 0); */
  world->add( new Triangle(A, C, B, ceilingMat));
  world->add( new Triangle(C, A, D, ceilingMat));

  // left
  Material* leftMat = new Lambertian(new ConstantTexture(1.f, 0.f, 0.f));
  world->add( new Triangle(A, B, E, leftMat));
  world->add( new Triangle(B, F, E, leftMat));

  // right
  Material* rightMat = new Lambertian(new ConstantTexture(0.f, 1.f, 0.f));
  world->add( new Triangle(D, H, C, rightMat));
  world->add( new Triangle(C, H, G, rightMat));

  // light
  float lightIntensity = 10.f;
  float lightSize = 0.3f;

  Material* lightMat = new Light(new ConstantTexture(lightIntensity, lightIntensity, lightIntensity));
  Vector lA = Vector(-lightSize, 0.99f, -lightSize);
  Vector lB = Vector(-lightSize, 0.99f, lightSize);
  Vector lC = Vector(lightSize, 0.99f, lightSize);
  Vector lD = Vector(lightSize, 0.99f, -lightSize);
  world->add( new Triangle(lA, lC, lB, lightMat));
  world->add( new Triangle(lC, lA, lD, lightMat));
  /* world->add( new Sphere(Vector(0, 0, 0), 0.2, new Light(new ConstantTexture(20.f, 20.f, 20.f))) ); */


  // stuff inside the box

  /* world->add( box(Vector(-0.7, -0.2, 0.2), 0.5, 0.5, 0.5, new Metal(new ConstantTexture(0.9, 0.9, 0.9), 0.01)) ); */
  /* for(int i=0 ; i<100 ; i++){ */
    world->extend( box(new Lambertian(new ConstantTexture(0.1, 0.2, 0.95)),
                    Vector(-0.6, -0.1, 0.1),
                    Vector(0.5, 0.8, 0.5),
                    Vector(0.f, -20.f, 0.f))->list );

    world->extend( box(new Lambertian(new ConstantTexture(1, 1, 1)),
          Vector(0., -0.5, -0.7),
          Vector(0.5, 0.5, 0.5),
          Vector(0.f, 15.f, 0.f))->list );
  /* } */


  // obj loading tests
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/


    /* Material* objMat = new Lambertian(new ConstantTexture(0.1f, 0.2f, 0.8f)); */
  Material* objMat = new Metal(new ConstantTexture(0.6f, 0.6f, 0.2f), 0.1f);
  /* Material* objMat = new Light(new ConstantTexture(0.1f, 0.1f, 0.8f)); */

  std::vector<Vector> vertices;
  std::vector<Vector> normals;
  /* std::vector<Vector> texturecoords; */
  std::vector<Vector> faces;  // computed normals indexed by vertices
  std::vector<int> nfaces;  // number of faces by vertices - to average out the normal

  ObjectGroup *obj = new ObjectGroup();
  std::vector<SmoothedTriangle*> tosmooth;

  std::ifstream file("cow.obj");
  /* std::ifstream file("bunny.obj"); */
  std::string str;
  while (std::getline(file, str)) {

    bool gotnormals = false;

    // vertices
    if( str.find("v ") == 0){
      float a, b, c;
      sscanf(str.c_str(), "v %f %f %f", &a, &b, &c);
      vertices.push_back(Vector(a, b, c));

      faces.push_back(VECTOR_ZERO);
      nfaces.push_back(0);

    // vertice normals
    } else if( str.find("vn ") == 0){
        float na, nb, nc;
        sscanf(str.c_str(), "vn %f %f %f", &na, &nb, &nc);
        normals.push_back(Vector(na, nb, nc));
        /* std::cout << Vector(na, nb, nc).length() << " na=" << na << " nb=" << nb << " nc=" << nc << std::endl; */


    // faces (ie triangles)
    } else if( str.find("f ") == 0){
      long a, b, c;
      long na, nb, nc;
      if(str.find("//") != std::string::npos){
        gotnormals = true;
        sscanf(str.c_str(), "f %ld//%ld %ld//%ld %ld//%ld", &a, &na, &b, &nb, &c, &nc);
      } else if(str.find("/") != std::string::npos) {
        gotnormals = true;
        sscanf(str.c_str(), "f %ld/%*s/%ld %ld/%*s/%ld %ld/%*s/%ld", &a, &na, &b, &nb, &c, &nc);
      } else {
        sscanf(str.c_str(), "f %ld %ld %ld", &a, &b, &c);
      }

      unsigned long A, B, C;
      A = a > 0 ? a-1 : vertices.size()+a;
      B = b > 0 ? b-1 : vertices.size()+b;
      C = c > 0 ? c-1 : vertices.size()+c;

      if(A > vertices.size()
          || B > vertices.size()
          || C > vertices.size()){
        std::cerr << "NOPE vertices " << A  << " " << B << " "  << C << " "  << vertices.size() << std::endl;
        std::cerr << str << std::endl;
        exit(1);
      }

      if(gotnormals) {
        unsigned long NA, NB, NC;
        NA = na > 0 ? na-1 : normals.size()+na;
        NB = nb > 0 ? nb-1 : normals.size()+nb;
        NC = nc > 0 ? nc-1 : normals.size()+nc;

        if(NA > normals.size()
            || NB > normals.size()
            || NC > normals.size()){
          std::cerr << "NOPE normals " << NA  << " " << NB << " "  << NC << " "  << normals.size() << std::endl;
          std::cerr << str << std::endl;
          exit(1);
        }

        obj->add( new SmoothedTriangle(  vertices[A],
                                         vertices[B],
                                         vertices[C],
                                         normals[NA],
                                         normals[NB],
                                         normals[NC],
                                         objMat) );
      } else {
        SmoothedTriangle* triangle = new SmoothedTriangle(
             vertices[A],
             vertices[B],
             vertices[C],
             objMat);
        tosmooth.push_back(triangle);

        faces[A] += triangle->norm;
        faces[B] += triangle->norm;
        faces[C] += triangle->norm;

        nfaces[A] += 1;
        nfaces[B] += 1;
        nfaces[C] += 1;
      }

    }
  }

  // smooth triangles we dont have the normal for
  for(unsigned int ivertice=0 ; ivertice<vertices.size() ; ivertice++){
    Vector vertice = vertices[ivertice];
    Vector norm = faces[ivertice]/nfaces[ivertice];
    for(SmoothedTriangle* triangle: tosmooth){
      if(vertice == triangle->a)
        triangle->na = norm;
      if(vertice == triangle->b)
        triangle->nb = norm;
      if(vertice == triangle->c)
        triangle->nc = norm;
    }
  }

  for(SmoothedTriangle* triangle: tosmooth)
    obj->add(triangle);


  /* world = new ObjectGroup(); */
  world->extend(obj->list);


  // debug cube
  /* world = new ObjectGroup(); */
  /* world->extend( box(new Metal(new ConstantTexture(0.8, 0.8, 0.8), 0.f))->list ); */


  // convert world to a BHV
  /* BHV* bhv_world = new BHV(&world->list[0], world->list.size()); */
  BHV* bhv_world = new BHV(world->list);
  std::cout << "Triangle count: " << world->list.size() << std::endl;
  std::cout << "BHV depth: " << bhv_world->depth() << std::endl;

  /* std::cout << bhv_world->str() << std::endl; */
  /* std::cout << bhv_world->bounding_box()->str() << std::endl; */
  /* std::cout << bhv_world->left->bounding_box()->str() << std::endl; */
  /* std::cout << bhv_world->right->bounding_box()->str() << std::endl; */




  auto bhvTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> totalBHVtime = bhvTime - startTime;
  std::cout << "\nBHV in: " << totalBHVtime.count() << " ms" << std::endl;
  startTime = std::chrono::high_resolution_clock::now();


  // start job threads
  std::thread *threads = new std::thread[nthreads];
  Vector*** out = new Vector**[nthreads];
  for(int ithread=0 ; ithread<nthreads ; ithread++) {
    out[ithread] = new Vector*[width];
    for(int i=0 ; i<width ; i++)
      out[ithread][i] = new Vector[height];

    threads[ithread] = std::thread(rt_pass, camera, bhv_world, width, height, max_bounces, nsamples/nthreads, out[ithread]);
    /* threads[ithread] = std::thread(rt_pass, camera, world, width, height, max_bounces, nsamples/nthreads, out[ithread]); */
  }


  long totalDirectRay = width*height*(nsamples-nsamples%nthreads);
  int perc = 0;
  long lastTotalRay = 0;
  int pauseCount = 0;  // keep track of the pause number: allow to display average rays for the last second - TODO rolling second
  const int pauseTime = 1; // in ms

  while(perc != 100) {

    perc = (nDirectRay*100)/totalDirectRay;

    std::this_thread::sleep_for(std::chrono::milliseconds(pauseTime));

    pauseCount++;
    if(pauseCount*pauseTime>=1000){
      pauseCount = 0;
      std::cout << "\r" << perc << " %" << "  -- " << (nTotalRay-lastTotalRay)/1000.f << "K ray per sec" << std::flush;
      lastTotalRay = nTotalRay;
    }

  }


  // join threads - just in case
  for(int ithread=0 ; ithread<nthreads ; ithread++)
    threads[ithread].join();


  // average res & write the image
  for(int j=height-1 ; j>=0; j--)
    for(int i=width-1 ; i>=0 ; i--) {
      Vector average = VECTOR_ZERO;
      for(int ithread=0 ; ithread<nthreads ; ithread++)
        average += out[ithread][i][j];
      average = average/nthreads;
      int *rgb = average.tone_map().toRGB(2);
      image << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
    }

  image.close();

  auto endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> totalTime = endTime - startTime;
  std::cout << "\n\nTotal: " << totalTime.count() << " ms" << std::endl;
  std::cout << "\nTriangle intersections: " << nTriangleIntersection/1000000.f << "M" << std::endl;
  std::cout << "BHV intersection: " << nBoxIntersection/1000000.f << "M" << std::endl;

  std::cout << "\nTriangle intersection per ray: " << nTriangleIntersection/float(nTotalRay) << std::endl;
  std::cout << "BHV intersection per ray: " << nBoxIntersection/float(nTotalRay) << std::endl;

  std::cout << "\nAverage bounce: " << nTotalRay/float(nDirectRay) << std::endl;

  std::cout << "\nDirect rays: " << nDirectRay << std::endl;
  std::cout << "Total rays: " << nTotalRay << std::endl;
  std::cout << "\nRay per seconds: " << nTotalRay/totalTime.count() << " k" << std::endl;

  return 0;
}
