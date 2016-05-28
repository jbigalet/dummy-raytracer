#ifndef DEF_UTILS
#define DEF_UTILS

#include <random>

/* #define RANDOM_FLOAT ((float)rand()/RAND_MAX) */

float random_float();
float random_float_2pi();
float random_float_minus_one();
#define RANDOM_FLOAT random_float()

#endif /* DEF_UTILS */
