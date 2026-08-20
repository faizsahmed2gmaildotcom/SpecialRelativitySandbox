#ifndef RELATIVITY
#define RELATIVITY
#include <cmath>
#include <iostream>
using vec3 = Vector<double, 3>;


constexpr double C = 1.0; // 299792458.0;
constexpr double C2 = C * C;


inline double getLF(const vec3 &v) {
    return 1.0 / std::sqrt(1 - v.magnitude2() / C2);
}

inline double betaLF(const vec3 &B) {
    return 1.0 / std::sqrt(1 - B.magnitude2());
}

inline vec3 betaRel(const vec3 &B_ref, const vec3 &B_obj) {
    const double LF_ref = betaLF(B_ref);
    return (B_obj * (1.0 / LF_ref) - B_ref + B_ref * (LF_ref / (LF_ref + 1) * (B_ref * B_obj))) * (1.0 / (1 - B_ref * B_obj));
}

struct VtxRefFrame {
    vec3 acc;
    vec3 vel;

    VtxRefFrame() = default;

    VtxRefFrame(const vec3 &acc, const vec3 &vel) : acc(acc), p(vel) {
        p *= getLF(vel);
    }

    virtual ~VtxRefFrame() = default;

    virtual void process(const double dt) {
        p += acc * dt;
        const double B2 = p.magnitude2() / C2;
        vel = p * (1.0 / std::sqrt(1 + B2));
    }

private:
    vec3 p; // Relativistic momentum (mass disregarded)
};

struct RefFrame : VtxRefFrame {
    vec3 pos;

    RefFrame() = default;

    RefFrame(const vec3 &acc, const vec3 &vel, const vec3 &pos) : VtxRefFrame(acc, vel), pos(pos) {
    }

    void process(const double dt) override {
        VtxRefFrame::process(dt);
        pos += vel * dt;
    }
};

struct CamRefFrame : VtxRefFrame {
    vec3 pos;

    CamRefFrame() = default;

    CamRefFrame(const vec3 &acc, const vec3 &vel, const vec3 &pos) : VtxRefFrame(acc, vel), pos(pos) {
    }

    void process(const double dt) override {
        VtxRefFrame::process(dt);
        pos += vel * dt * getLF(vel); // Length contraction
    }
};


#endif
