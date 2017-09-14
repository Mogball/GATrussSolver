#ifndef GA_TRUSSER_GLOBAL_H
#define GA_TRUSSER_GLOBAL_H

#include <cstdlib>
#include <time.h>
#include "Point.h"

typedef int int32;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;

inline float64 rand_float(float64 lower, float64 upper) {
	float64 r = static_cast <float64> (rand()) / static_cast <float64> (RAND_MAX);
	float64 range = upper - lower;
	return r * range + lower;
}

inline int32 rand_int(int32 lower, int32 upper) {
	float64 r = rand_float(lower, upper);
	return (int32) floor(r);
}

inline bool rand_bit() {
	return rand_float(0, 1) < 0.5;
}

inline Point unit_vector(Point *nodes, uint32 i, uint32 j) {
	Point r = nodes[j] - nodes[i];
	return r / r.norm();
}

inline float64 node_length(Point *nodes, uint32 i, uint32 j) {
	Point r = nodes[j] - nodes[i];
	return r.norm();
}

namespace Global {
	inline void init() {
		srand((unsigned int) time(NULL));
	}
}

#endif //GA_TRUSSER_GLOBAL_H
