#ifndef SPECIAL_RELATIVITY
#define SPECIAL_RELATIVITY
#include "vector_matrix.h"
#include "relativity.h"

struct RefFrame {
    Vector<3> pos;
    Vector<3> vel;
    Vector<3> acc;
};

template<int vts>
class BaseObj {
    RefFrame frame;
    Matrix<vts, 3> world_vertices;
    Matrix<vts, 3> cam_vertices; // Lorentz-transformed result of origin+world_vertices

    void relUpdateFrame(const double dt) {
        const Vector v_rel = frame.vel * LF(frame.vel);
        const Vector v_new = v_rel + frame.acc * dt;
        frame.vel = v_new * (1.0 / std::sqrt(1 + v_new.magnitude2() / C2));
        frame.pos += frame.vel * dt;
    }

    void relUpdateVertex(const double dt, const RefFrame &cam) {
        const Vector beta = cam.vel * (1.0 / C2);
        for (int v = 0; v < vts; v++) {

        }
    }

public:
    BaseObj() = default;

    virtual ~BaseObj() = default;

    BaseObj(const Vector<3> &pos, const Matrix<vts, 3> &vertices, const Vector<3> &vel = {}, const Vector<3> &acc = {})
        : frame(pos, vel, acc), world_vertices(vertices) {
    }

    // Runs every frame
    void process(const double dt) {
        relUpdateFrame(dt);
    }
};

#endif
