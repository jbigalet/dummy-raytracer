#include <iostream>
#include "vector.h"

int main() {
  int height = 100;
  int width = 200;
  std::cout << "P3\n" << width << " " << height << "\n255\n";
  for(int j=0 ; j<height ; j++)
    for(int i=0 ; i<width ; i++) {
      std::cout << 30 << " " << 150 << " " << 20 << "\n";
    }
}
