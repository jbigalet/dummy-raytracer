#include "utils.h"

#include <chrono>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::uniform_real_distribution<float> dist_float(0.0f, 1.0f);
std::minstd_rand rng_minstd(seed);

float random_float() {
  return dist_float(rng_minstd);
}
