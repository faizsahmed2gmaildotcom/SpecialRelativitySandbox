#ifndef SPECIALRELATIVITYSANDBOX_CONFIG_H
#define SPECIALRELATIVITYSANDBOX_CONFIG_H
#include <limits>

// dtypes
using vec3 = Array<double, 3>;
using range = std::pair<unsigned int, unsigned int>;
using tris = std::vector<Array<unsigned, 3> >;

// invalid vals
constexpr float INVALID_FLOAT = std::numeric_limits<float>::infinity();
const Array<float, 3> INVALID_ARR3{INVALID_FLOAT, INVALID_FLOAT, INVALID_FLOAT};
constexpr double INVALID_DOUBLE = std::numeric_limits<double>::infinity();
const vec3 INVALID_VEC3{INVALID_DOUBLE, INVALID_DOUBLE, INVALID_DOUBLE};

// constants
#define MAX_PAST_TIME 60  // seconds
constexpr double C = 299792458.0;
constexpr double C2 = C * C;
constexpr int fps = 60;
constexpr double spf = 1.0 / fps;
inline double time_since_last_frame = 0.0;
constexpr double WORLD_PLANE_Y = -3.0;

#endif //SPECIALRELATIVITYSANDBOX_CONFIG_H
