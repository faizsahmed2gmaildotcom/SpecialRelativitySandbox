#ifndef RELATIVITY
#define RELATIVITY
#include <cmath>
#include <iostream>
#include "config.h"

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

struct FrameState {
    vec3 vel;
    vec3 pos;

    FrameState() : vel(INVALID_VEC3), pos(INVALID_VEC3) {
    }

    FrameState(const vec3 &vel, const vec3 &pos) : vel(vel), pos(pos) {
    }
};

const FrameState INVALID_FRAME;

template<typename T, int len>
class CircularStack {
    T *arr;
    int head;

public:
    explicit CircularStack() : arr(new T[len]{}), head(len - 1) {
    }

    void append(const T &val) {
        head = (head + 1) % len;
        arr[head] = val;
    }

    T &operator[](const int idx) {
        int r_idx = head + idx;
        if (r_idx < 0) r_idx += len;
        return arr[r_idx];
    }

    const T &operator[](const int idx) const {
        int r_idx = head + idx;
        if (r_idx < 0) r_idx += len;
        return arr[r_idx];
    }

    void setAll(const T &val) {
        for (int i = 0; i < len; i++) arr[i] = val;
    }

    ~CircularStack() {
        delete[] arr;
    }
};


struct CMRefFrame {
    vec3 vel;
    vec3 pos;
    vec3 acc;
    double theta{};
    double phi{};
    double omega{};

    CMRefFrame() = default;

    CMRefFrame(const vec3 &acc, const vec3 &vel, const vec3 &pos) : pos(pos), acc(acc), p(vel) {
        if (vel.magnitude() > C) std::cerr << "VELOCITY GREATER THAN C!\n";
        p *= getLF(vel);
    }

    virtual ~CMRefFrame() = default;

    virtual void process(const double dt) {
        p += acc * dt;
        const double B2 = p.magnitude2() / C2;
        vel = p * (1.0 / std::sqrt(1 + B2));
        pos += vel * dt;
    }

    void setVelocity(const vec3 &new_vel) {
        if (new_vel.magnitude() > C) std::cerr << "VELOCITY GREATER THAN C!\n";
        p = new_vel * getLF(new_vel);
    }

private:
    vec3 p; // Relativistic momentum (mass disregarded)
};


struct CamRefFrame : CMRefFrame {
    CamRefFrame() = default;

    CamRefFrame(const vec3 &acc, const vec3 &vel, const vec3 &pos) : CMRefFrame(acc, vel, pos) {
    }

    void process(const double dt) override {
        CMRefFrame::process(dt);
        // pos += vel * dt * (getLF(vel) - 1.0); // Length contraction along motion
    }
};

class BaseObj;

struct VtxRefFrame {
    vec3 pos;
    vec3 vel;
    const BaseObj *owner{};

    void saveState() {
        history.append({vel, pos});
    }

    [[nodiscard]] double tRet(const CamRefFrame &cam) const {
        // For accurate tRet, go through the circular array until (cam.pos - frame.pos)/c - t_ret*fps ~= 0
        const vec3 dx = cam.pos - pos;
        const vec3 v_rel = cam.vel - vel;

        const double a = v_rel.magnitude2() - C2;
        const double b = 2.0 * (dx * v_rel);
        const double c = dx.magnitude2();
        const double discriminant = b * b - 4.0 * a * c;
        const double sqrt_disc = std::sqrt(discriminant);

        const double t1 = (-b + sqrt_disc) / (2.0 * a);
        const double t2 = (-b - sqrt_disc) / (2.0 * a);
        const double t_ret = t1 < 0.0 ? t1 : t2;

        return t_ret;
    }

    Array<float, 3> getOptPos(const CamRefFrame &cam, const double t_ret) {
        const vec3 x_emit = getState(t_ret).pos;
        if (x_emit == INVALID_VEC3) return INVALID_ARR3;

        // Lorentz boost the emission event into the camera rest frame
        const vec3 x_rel = x_emit - cam.pos;
        const vec3 V = cam.vel;
        const double V2 = V.magnitude2();

        vec3 x_new;
        if (V2 > 1e-12) {
            const double LF_cam = getLF(cam.vel);
            const double v_dot_x = V * x_rel;
            const double scale = (LF_cam - 1.0) * v_dot_x / V2 - LF_cam * t_ret;
            x_new = x_rel + V * scale;
        } else {
            x_new = x_rel;
        }

        return {static_cast<float>(x_new[0]), static_cast<float>(x_new[1]), static_cast<float>(x_new[2])};
    }

    FrameState getState(const double t_ret) {
        const int lower_t = std::floor(t_ret * fps);
        const int upper_t = std::ceil(t_ret * fps);
        // std::cout << lower_t << " | " << upper_t << "\n";
        if (lower_t < -MAX_PAST_TIME * fps) return INVALID_FRAME; // Outside view distance
        const vec3 state_pos = history[lower_t].pos.interpolate(history[upper_t].pos);
        const vec3 state_vel = history[lower_t].vel.interpolate(history[upper_t].vel);
        return {state_vel, state_pos};
    }

    void resetHistory() {
        history.setAll({vel, pos});
    }

private:
    CircularStack<FrameState, MAX_PAST_TIME * fps> history; // Visual state at history[fps * t_ret]
};


#endif
