#include <iostream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <assert.h>

#include "stats.h"
#include "vector.h"

#include "material.h"
#include "ray.h"
#include "object.h"
#include "camera.h"
#include "texture.h"

void rt_pass(Camera camera, const BHV& world, int width, int height, int startingWidth, int max_bounces, int nsamples, Vector** out) {
  for(int i=0 ; i<width ; i++){
    /* std::cout << i << std::endl; */
    for(int j=0 ; j<height ; j++)
      out[i][j] = camera.getColor(world, i%width, j, max_bounces, nsamples);
  }
}

int main() {

  assert(sizeof(BHVNode)==32);  // in case we mess it up one day


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
  /* int height = 1080/4; */
  /* int height = 50; */

  /* int width = 1920/2; */
  /* int height = 1080/2; */

  /* int width = 1920; */
  int height = 1080;

  /* int height = 1920; */
  /* int width = 1080; */

  /* int width = 1200; */
  /* int height = 600; */

  /* int width = 600; */
  /* int height = 300; */

  int width = height;

  /* int nsamples = 1; */
  /* int nsamples = 4; */
  int nsamples = 20;
  /* int nsamples = 50; */
  /* int nsamples = 100; */
  /* int nsamples = 200; */
  /* int nsamples = 500; */
  /* int nsamples = 1000; */
  /* int nsamples = 2000; */
  /* int nsamples = 5000; */
  /* int nsamples = 10000; */

  /* int max_bounces = 1; */
  /* int max_bounces = 2; */
  int max_bounces = 5;
  /* int max_bounces = 10; */
  /* int max_bounces = 100; */

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

#if 0

  // dragon
  Camera camera(
      width,
      height,
      Vector(0.f, 0.1f, 0.22f),  // profile
      /* Vector(-0.072f, 0.14f, -0.03f),  // quarter zoomed in */
      /* Vector(-0.2f, 0.2f, -0.2f),  // quarter */
      Vector(0.f, 0.12f, 0),
      Vector(0, 1, 0),
      40
      );

#elif 0

  /* // buddha */
  Camera camera(
      width,
      height,
      Vector(0.f, 0.2f, 0.35f),  // quarter
      Vector(0.f, 0.15f, 0),
      Vector(0, 1, 0),
      40
      );

#endif

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
  //
    auto checkpoint = std::chrono::high_resolution_clock::now();

    Material* objMat = new Lambertian(new ConstantTexture(0.1f, 0.2f, 0.8f));
  /* Material* objMat = new Metal(new ConstantTexture(0.6f, 0.6f, 0.2f), 0.1f); */
  /* Material* objMat = new Light(new ConstantTexture(0.1f, 0.1f, 0.8f)); */

  std::vector<Vector> vertices;
  std::vector<Vector> normals;
  /* std::vector<Vector> texturecoords; */
  std::vector<std::array<long double, 3>> faces;  // computed normals indexed by vertices
  std::vector<int> nfaces;  // number of faces by vertices - to average out the normal
  std::vector<std::array<int, 3>> faceLinks;  // for each face, link to vertice ids

  ObjectGroup *obj = new ObjectGroup();
  std::vector<SmoothedTriangle*> tosmooth;

  int ignoredObj = 0;

#if 1  // obj loading

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

      faces.push_back( {{0.L, 0.L, 0.L}} );
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

        long axis[] = {a, b, c};
        for(long axes : axis){
          for(int i=0 ; i<3 ; i++)
            faces[axes][i] += (long double) triangle->norm[i];

          nfaces[axes] += 1;
        }

        std::array<int, 3> link { {(int)a, (int)b, (int)c} };
        faceLinks.push_back(link);
      }

    }
  }

  long face_count = faces.size();

#else  // ply loading  -- pretty dumb one
  // assume the only elements are vertices & faces
  // + faces are composed of exactly 3 vertices
  // + ascii format

  std::ifstream file("ply/dragon_recon/dragon_vrip.ply");
  /* std::ifstream file("ply/dragon_recon/dragon_vrip_res2.ply"); */

  /* std::ifstream file("ply/happy_recon/happy_vrip.ply"); */
  /* std::ifstream file("ply/happy_recon/happy_vrip_res4.ply"); */

  std::string str;

  long face_count = -1;
  long vertex_count = -1;

  // read header
  while(std::getline(file, str)){
    if(str == "ply")
      continue;  // ignore

    else if(str.find("format") == 0){
      assert(str == "format ascii 1.0");  // only format supported atm

    } else if(str.find("element") == 0){
      if(str.find("element vertex") == 0) {
        assert(face_count == -1);
        sscanf(str.c_str(), "element vertex %ld", &vertex_count);

      } else if(str.find("element face") == 0) {
        assert(vertex_count != -1);
        sscanf(str.c_str(), "element face %ld", &face_count);

      } else {
        std::cout << "unknow element type in ply: " << str << std::endl;
        exit(1);
      }


    } else if(str.find("property") == 0){
      continue;  // ignore atm - assume vertices are x,y,z floats ; faces as 3 ints

    } else if(str.find("comment") == 0) {
      continue;  // ignore

    } else if(str == "end_header"){
      break;

    } else {
      std::cout << "UNKNOWN LINE IN PLY: " << str << std::endl;
    }
  }

  std::cout << "PLY header: vertices = " << vertex_count << " ; faces = " << face_count << std::endl;

  std::cout << "PLY header loaded in: " << (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - checkpoint)).count() << " ms" << std::endl;
  checkpoint = std::chrono::high_resolution_clock::now();

  vertices.reserve(vertex_count);
  faces.reserve(vertex_count);
  nfaces.reserve(vertex_count);
  /* faceLinks.reserve(face_count); */
  /* tosmooth.reserve(face_count); */

  std::cout << "PLY struct reservation in: " << (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - checkpoint)).count() << " ms" << std::endl;
  checkpoint = std::chrono::high_resolution_clock::now();

  for(int ivertex=0 ; ivertex < vertex_count ; ivertex++){
    std::getline(file, str);
    float a, b, c;
    sscanf(str.c_str(), "%f %f %f", &a, &b, &c);
    vertices.push_back(Vector(a,b,c));

    faces.push_back( {{0.L, 0.L, 0.L}} );
    nfaces.push_back(0);
  }

  std::cout << "PLY vertices loaded in: " << (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - checkpoint)).count() << " ms" << std::endl;
  checkpoint = std::chrono::high_resolution_clock::now();

  for(int iface=0 ; iface < face_count ; iface++){
    std::getline(file, str);
    long a, b, c;
    sscanf(str.c_str(), "3 %ld %ld %ld", &a, &b, &c);

    /* std::cout << "str \"" << str << "\"  => a: " << a << " b: " << b << " c: " << c << std::endl; */

    assert(a >= 0 && b >= 0 && c >= 0
        && a < vertex_count
        && b < vertex_count
        && c < vertex_count);

    SmoothedTriangle* triangle = new SmoothedTriangle(
      vertices[a],
      vertices[b],
      vertices[c],
      objMat);

    // check for NaN in file
    bool toignore = false;
    for(int iaxis=0 ; iaxis<3 ; iaxis++)
      if(isnan(triangle->norm[iaxis]) || triangle->norm[iaxis] < -1.1f) {
        toignore = true;
        /* std::cout << "problem in ply file - incorrect normals: " << triangle->norm << " vertices: a=" << vertices[a] << " b=" << vertices[b] << " c= " << vertices[c] << "  indices: (" << a << ", " << b << ", " << c << ")" << std::endl; */
      }

    if(!toignore) {
      long axis[] = {a, b, c};
      for(long axes : axis){
        for(int i=0 ; i<3 ; i++)
          faces[axes][i] += (long double) triangle->norm[i];

        nfaces[axes] += 1;
      }

      tosmooth.push_back(triangle);

      std::array<int, 3> link { {(int)a, (int)b, (int)c} };
      faceLinks.push_back(link);

    } else {
      ignoredObj++;
    }
  }

  std::cout << "PLY faces loaded in: " << (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - checkpoint)).count() << " ms" << std::endl;
  checkpoint = std::chrono::high_resolution_clock::now();

#endif

  if(ignoredObj > 0)
    std::cout << "\n=============\n!!!! " << ignoredObj << " FACES HAVE BEEN IGNORED ON THE 1st PASS!!!\n==========\n" << std::endl;

  int secondPassIgnoredObj = 0;

  file.close();

  for(unsigned int iface=0 ; iface<face_count-ignoredObj ; iface++){
    SmoothedTriangle* triangle = tosmooth[iface];
    std::array<int, 3> link = faceLinks[iface];

    for(int iaxes = 0 ; iaxes<3 ; iaxes++){
      triangle->na[iaxes] = float( faces[link[0]][iaxes] / nfaces[link[0]] );
      triangle->nb[iaxes] = float( faces[link[1]][iaxes] / nfaces[link[1]] );
      triangle->nc[iaxes] = float( faces[link[2]][iaxes] / nfaces[link[2]] );
    }

    /* std::cout << "link: " << link[0] << " " << link[1] << " " << link[2] << std::endl; */

    bool ignoreobj = false;  // if some problem occurs
    for(int iaxis=0 ; iaxis<3 ; iaxis++){
      /* bool toignorethis = true; */
      /* if(nfaces[link[iaxis]] == 0) */
      /*   std::cout << "problem in ply file: " << link[iaxis] << std::endl; */

      /* else if(isnan(triangle->na[iaxis]) || triangle->na[iaxis] < -1.1f) */
      /*   std::cout << "problem in ply file: " << faces[link[0]] << std::endl; */

      /* else if(isnan(triangle->nb[iaxis]) || triangle->nb[iaxis] < -1.1f) */
      /*   std::cout << "problem in ply file: " << faces[link[1]] << std::endl; */

      /* else if(isnan(triangle->nc[iaxis]) || triangle->nc[iaxis] < -1.1f) */
      /*   std::cout << "problem in ply file: " << faces[link[2]] << std::endl; */

      /* else */
      /*   toignorethis = false; */

      /* if(toignorethis) */
      /*   ignoreobj = true; */

      if( !(nfaces[link[iaxis]] != 0
          && !isnan(triangle->na[iaxis]) && triangle->na[iaxis] > -1.f
          && !isnan(triangle->nb[iaxis]) && triangle->nb[iaxis] > -1.f
          && !isnan(triangle->nc[iaxis]) && triangle->nc[iaxis] > -1.f))

        ignoreobj = true;
    }

    if(!ignoreobj)
      obj->add(triangle);
    else
      secondPassIgnoredObj++;
  }

  std::cout << "Triangle smoothing in: " << (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - checkpoint)).count() << " ms" << std::endl;

  auto fileReadingTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> totalFileReadingTime = fileReadingTime - startTime;
  std::cout << "File loaded in: " << totalFileReadingTime.count() << " ms\n" << std::endl;

  if(secondPassIgnoredObj > 0)
    std::cout << "\n=============\n!!!! " << secondPassIgnoredObj  << " FACES HAVE BEEN IGNORED ON 2nd PASS!!!\n==========\n" << std::endl;

  /* world = new ObjectGroup(); */
  world->extend(obj->list);


  // debug cube
  /* world = new ObjectGroup(); */
  /* world->extend( box(new Metal(new ConstantTexture(0.8, 0.8, 0.8), 0.f))->list ); */


  // convert world to a BHV
  auto bhvStartTime = std::chrono::high_resolution_clock::now();

  /* BHV* bhv_world = new BHV(&world->list[0], world->list.size()); */
  const BHV bhv_world(world->list);

  std::cout << "Triangle count: " << world->list.size() << std::endl;
  /* std::cout << "BHV depth: " << bhv_world.depth() << std::endl; */

  std::cout << "\nWorld center at: " << bhv_world.bounding_box()->getCenter().str() << std::endl;
  std::cout << "World bounding box: " << bhv_world.bounding_box()->str() << std::endl;

  /* std::cout << bhv_world->str() << std::endl; */
  /* std::cout << bhv_world->bounding_box()->str() << std::endl; */
  /* std::cout << bhv_world->left->bounding_box()->str() << std::endl; */
  /* std::cout << bhv_world->right->bounding_box()->str() << std::endl; */


  auto bhvTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> totalBHVtime = bhvTime - bhvStartTime;
  std::cout << "\nBHV in: " << totalBHVtime.count() << " ms" << std::endl;
  startTime = std::chrono::high_resolution_clock::now();


  // start job threads
  std::thread *threads = new std::thread[nthreads];
  Vector*** out = new Vector**[nthreads];
  for(int ithread=0 ; ithread<nthreads ; ithread++) {
    out[ithread] = new Vector*[width];
    for(int i=0 ; i<width ; i++)
      out[ithread][i] = new Vector[height];

    threads[ithread] = std::thread(rt_pass, camera, bhv_world, width, height, (ithread*width)/nthreads, max_bounces, nsamples/nthreads, out[ithread]);
    /* threads[ithread] = std::thread(rt_pass, camera, bhv_world, width, height, 0, max_bounces, nsamples/nthreads, out[ithread]); */
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

  auto endTime = std::chrono::high_resolution_clock::now();

  // average res & write the image
  std::ofstream image;
  image.open("image.ppm");
  /* image.open("image_"+std::to_string(nsamples)+".ppm"); */

  image << "P3\n" << width << " " << height << "\n255\n";

  // debug: bhv traversal count
  // compute maximum to display the right color panel
#if BHV_TRAVERSAL_COUNT
  int max_traversal = 0;
  long double average_traversal = 0.f;
  for(int j=height-1 ; j>=0; j--)
    for(int i=width-1 ; i>=0 ; i--)
      for(int ithread=0 ; ithread<nthreads ; ithread++){
        if(out[ithread][i][j].x > max_traversal)
           max_traversal = out[ithread][i][j].x;
        average_traversal += out[ithread][i][j].x;
      }

  std::cout << "\nmax traversal: " << max_traversal << std::endl;
  std::cout << "average direct traversal: " << (average_traversal/totalDirectRay) << std::endl;
#endif

  for(int j=height-1 ; j>=0; j--)
    for(int i=width-1 ; i>=0 ; i--) {
      Vector average = VECTOR_ZERO;
      for(int ithread=0 ; ithread<nthreads ; ithread++)
        average += out[ithread][i][j];
      average = average/nthreads;

#if BHV_TRAVERSAL_COUNT
      int *rgb = (average/max_traversal).toRGB(2);
#else
      int *rgb = average.tone_map().toRGB(2);
#endif
      // check for incoherent values
      for(int iaxis=0 ; iaxis<3 ; iaxis++)
        if(rgb[iaxis] < 0 || rgb[iaxis] > 255 || isnan(rgb[iaxis])){
          std::cout << "something's wrong: " << average << std::endl;
          for(int ithread=0 ; ithread<nthreads ; ithread++)
            std::cout << "on thread " << ithread << " => " << out[ithread][i][j] << std::endl;
          rgb[iaxis] = 0;  // =[
        }

      image << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
    }

  image.close();

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
