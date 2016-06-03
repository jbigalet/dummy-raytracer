#ifndef DEF_STATS
#define DEF_STATS

#include <atomic>

extern std::atomic<long> nDirectRay;
extern std::atomic<long> nTotalRay;
extern std::atomic<long> nTriangleIntersection;
extern std::atomic<long> nBoxIntersection;

#endif /* DEF_STATS */
