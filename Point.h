//
// Created by Jeff Niu on 7/18/2017.
//

#ifndef GA_TRUSSER_POINT_H
#define GA_TRUSSER_POINT_H

#include <cmath>
#include <iostream>
#include "Global.h"

typedef double float64;
class Point {
public:
	float64 x;
	float64 y;

	inline Point() : x(0.0), y(0.0) {}

	inline explicit Point(float64 x, float64 y) : x(x), y(y) {}

	inline float64 norm() const {
		return sqrt(x * x + y * y);
	}
};

inline Point operator-(const Point &a, const Point &b) {
	return Point(a.x - b.x, a.y - b.y);
}

inline Point operator/(const Point &a, const float64 &s) {
	return Point(a.x / s, a.y / s);
}

inline Point operator*(const Point &a, const float64 &s) {
	return Point(a.x * s, a.y * s);
}

inline std::ostream &operator<<(std::ostream &os, const Point &v) {
	os << '(' << v.x << ',' << ' ' << v.y << ')';
}

#endif //GA_TRUSSER_POINT_H
