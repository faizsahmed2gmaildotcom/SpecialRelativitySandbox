#ifndef SPECIAL_RELATIVITY
#define SPECIAL_RELATIVITY
#include "vector_matrix.h"
#include "relativity.h"

template<int vts>
class BaseObj {
    Vector<VtxRefFrame, vts> vtx_frames;
    const Matrix<vts, 3> vertices;
    Matrix<vts, 3> world_vertices;
    Matrix<vts, 3> cam_vertices; // Lorentz-transformed result of pos+vertices

    void updateWorldVtc(const double dt, const int vtx) {
        const Vector v_rel = vtx_frames[vtx].vel * LF(vtx_frames[vtx].vel);
        const Vector v_new = v_rel + vtx_frames[vtx].acc * dt;
        vtx_frames[vtx].vel = v_new * (1.0 / std::sqrt(1 + v_new.magnitude2() / C2));
        for (int xyz = 0; xyz < 3; xyz++) world_vertices.get(vtx, xyz) += vtx_frames[vtx].vel[xyz] * dt;
    }

    void updateCamVtc() {

    }

    friend std::ostream &operator<<(std::ostream &os, const BaseObj &obj) {
        os << "Vertices (world):\n" << obj.world_vertices << '\n';
        os << "Vertices (camera):\n" << obj.cam_vertices << '\n';
        return os;
    }

public:
    explicit BaseObj(const Matrix<vts, 3> &vertices) : vertices(vertices), world_vertices(vertices) {
    }

    virtual ~BaseObj() = default;

    // Runs every frame
    void process(const double dt) {
        updateWorldVtc(dt);
    }

    void offset(const Vector<double, 3> &dx) {
        for (int v = 0; v < vts; v++)
            for (int xyz = 0; xyz < 3; xyz++)
                world_vertices.get(v, xyz) += dx[xyz];
    }
};

#endif
