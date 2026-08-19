#ifndef RELATIVITY
#define RELATIVITY
#include <cmath>
#include <iostream>

struct VtxRefFrame {
    Vector<double, 3> acc;
    Vector<double, 3> vel;
};

struct RefFrame : VtxRefFrame {
    Vector<double, 3> pos;
};

constexpr double C = 100.0; // 299792458.0;
constexpr double C2 = C * C;

inline double LF(const Vector<double, 3> &v) {
    return 1.0 / std::sqrt(1 - v.magnitude2() / C2);
}

inline double betaLF(const Vector<double, 3> &B) {
    return 1.0 / std::sqrt(1 - B.magnitude2());
}

inline Vector<double, 3> betaRel(const Vector<double, 3> &B_ref, const Vector<double, 3> &B_obj) {
    const double LF_ref = betaLF(B_ref);
    return (B_obj * (1.0 / LF_ref) - B_ref + B_ref * (LF_ref / (LF_ref + 1) * (B_ref * B_obj))) * (1.0 / (1 - B_ref * B_obj));
}

inline Vector<double, 3> vtxOffsetRel(const RefFrame &cam, const RefFrame &vtx) {
}

inline double getRelDt(const double dt, const Vector<double, 3> &rel_v) {
    const double rel_B = rel_v.magnitude() / C;
    return dt * std::sqrt((1 + rel_B) / (1 - rel_B));
}

#endif
