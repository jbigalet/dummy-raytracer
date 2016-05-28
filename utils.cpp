#include "utils.h"

#include <chrono>
#include <cmath>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::minstd_rand rng_minstd(seed);

std::uniform_real_distribution<float> dist_float(0.0f, 1.0f);
std::uniform_real_distribution<float> dist_float_minusone(-1.0f, 1.0f);
std::uniform_real_distribution<float> dist_float_2pi(0.0f, 2*M_PI);

float random_float() {
  return dist_float(rng_minstd);
}
float random_float_minus_one() {
  return dist_float_minusone(rng_minstd);
}
float random_float_2pi() {
  return dist_float_2pi(rng_minstd);
}
