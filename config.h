#ifndef SPECIALRELATIVITYSANDBOX_CONFIG_H
#define SPECIALRELATIVITYSANDBOX_CONFIG_H
#include <limits>
#define MAX_PAST_TIME 6000

using vec3 = Array<double, 3>;
constexpr double C = 10.0; //299792458.0;
constexpr double C2 = C * C;
constexpr double INVALID_VAL = std::numeric_limits<double>::infinity();
const vec3 INVALID_VECTOR{INVALID_VAL, INVALID_VAL, INVALID_VAL};
constexpr int fps = 60;
constexpr double spf = 1.0 / fps;
inline double time_since_last_frame = 0.0;

#endif //SPECIALRELATIVITYSANDBOX_CONFIG_H
