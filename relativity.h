#ifndef RELATIVITY
#define RELATIVITY
#include <cmath>
#include "objects.h"

constexpr double C = 299792458.0;
constexpr double C2 = C * C;

inline double LF(const Vector<3> &v) {
    return 1.0 / std::sqrt(1 - v.magnitude2() / C2);
}

#endif
