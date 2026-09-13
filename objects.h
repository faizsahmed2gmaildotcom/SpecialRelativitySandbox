#ifndef OBJECTS
#define OBJECTS
#include "vector_matrix.h"
#include "relativity.h"

struct ObjVtxDat {
    std::vector<VtxRefFrame> vtx_frames;
    std::vector<vec3 *> optical_vertices;
    const std::vector<vec3> vertices;

    explicit ObjVtxDat(const std::vector<vec3> &vertices, const std::vector<vec3 *> &alloc_opt_vts, const std::vector<WorldVtx *> &alloc_world_vts,
                       const BaseObj *parent)
        : optical_vertices(alloc_opt_vts), vertices(vertices) {
        if (alloc_opt_vts.size() != alloc_world_vts.size()) std::cerr << "Optical and world vertex lengths do not match!\n";

        vtx_frames.resize(alloc_world_vts.size());
        for (int i = 0; i < alloc_world_vts.size(); i++) {
            vtx_frames[i].vtx = alloc_world_vts[i];
            vtx_frames[i].vtx->owner = parent;
        }
    }

    size_t size() const {
        return vertices.size();
    }
};

class BaseObj {
    CMRefFrame main_frame; // Center of mass
    ObjVtxDat vtx_dat;

    void updateWorldVtc() const {
        // Update world_vertices[vtx] based on main_frame attributes
        for (int v = 0; v < vtx_dat.size(); v++)
            vtx_dat.vtx_frames[v].vtx->pos = main_frame.pos + vtx_dat.vertices[v];
    }

    friend std::ostream &operator<<(std::ostream &os, const BaseObj &obj) {
        os << "Vertices (world):\n";
        for (const auto &vtx_frame: obj.vtx_dat.vtx_frames)
            os << vtx_frame.vtx->pos << '\n';
        os << "Vertices (optical):\n";
        for (const auto &opt_v: obj.vtx_dat.optical_vertices)
            os << *opt_v << '\n';
        return os;
    }

public:
    explicit BaseObj(const std::vector<vec3> &vertices, const std::vector<vec3 *> &alloc_opt_vts, const std::vector<WorldVtx *> &alloc_world_vts)
        : vtx_dat(vertices, alloc_opt_vts, alloc_world_vts, this) {
    }

    virtual ~BaseObj() = default;

    void process(const double dt, const CamRefFrame &cam) {
        main_frame.process(dt);
        updateWorldVtc();
        if (time_since_last_frame != 0.0) return;
        for (int v = 0; v < vtx_dat.size(); ++v) {
            vtx_dat.vtx_frames[v].saveState();
            const double t_ret = vtx_dat.vtx_frames[v].tRet(cam);
            *vtx_dat.optical_vertices[v] = vtx_dat.vtx_frames[v].getOptPos(cam, t_ret);
        }
    }

    void offset(const vec3 &dx) {
        main_frame.pos += dx;
    }

    void resetHistory() {
        for (VtxRefFrame &vtx_frame: vtx_dat.vtx_frames) vtx_frame.resetHistory();
    }
};

#endif
