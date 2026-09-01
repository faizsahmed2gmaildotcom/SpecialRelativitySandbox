#ifndef SPECIAL_RELATIVITY
#define SPECIAL_RELATIVITY
#include "vector_matrix.h"
#include "relativity.h"

template<int vts>
class BaseObj {
    CMRefFrame main_frame; // Center of mass
    Vector<VtxRefFrame, vts> vtx_frames;
    const Matrix<vts, 3> vertices;
    Matrix<vts, 3> world_vertices; // Physical positions of vertices in world ref frame
    Matrix<vts, 3> optical_vertices; // Optical positions of vertices in cam ref frame

    void updateWorldVtc() {
        // Update world_vertices[vtx] based on main_frame attributes
        for (int v = 0; v < vts; v++)
            world_vertices[v] = main_frame.pos + vertices[v];
    }

    friend std::ostream &operator<<(std::ostream &os, const BaseObj &obj) {
        os << "Vertices (world):\n" << obj.world_vertices << '\n';
        os << "Vertices (optical):\n" << obj.optical_vertices << '\n';
        return os;
    }

public:
    explicit BaseObj(const Matrix<vts, 3> &vertices) : vertices(vertices), world_vertices(vertices) {
        for (int vtx = 0; vtx < vts; ++vtx)
            vtx_frames[vtx].world_pos = &world_vertices[vtx];
    }

    virtual ~BaseObj() = default;

    void process(const double dt, const CamRefFrame &cam) {
        main_frame.process(dt);
        updateWorldVtc();
        if (time_since_last_frame != 0.0) return;
        for (int v = 0; v < vts; ++v) {
            vtx_frames[v].saveState();
            const double t_ret = vtx_frames[v].tRet(cam);
            optical_vertices[v] = vtx_frames[v].getOptPos(cam, t_ret);
        }
    }

    void offset(const vec3 &dx) {
        main_frame.pos += dx;
    }

    void resetHistory() {
        for (VtxRefFrame &vtx_frame: vtx_frames) vtx_frame.resetHistory();
    }
};

#endif
