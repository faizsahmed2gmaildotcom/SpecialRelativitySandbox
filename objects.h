#ifndef OBJECTS
#define OBJECTS
#include "vector_matrix.h"
#include "relativity.h"

struct GlobalVertices {
    std::vector<Array<float, 3>> optical_vtc;  // float instead of double for GPU
    std::vector<VtxRefFrame> world_vtc;
    std::vector<Array<unsigned int, 3> > triangle_indices;

    range allocVtc(const int n) {
        range alloc_vtc = {optical_vtc.size(), optical_vtc.size() + n};
        optical_vtc.resize(optical_vtc.size() + n);
        world_vtc.resize(world_vtc.size() + n);
        return alloc_vtc;
    }

    range allocTris(const std::vector<Array<unsigned int, 3>> &new_triangle_indices) {
        range alloc_vtc = {triangle_indices.size(), triangle_indices.size() + new_triangle_indices.size()};
        triangle_indices.reserve(triangle_indices.size() + new_triangle_indices.size());
        triangle_indices.append_range(std::views::as_rvalue(new_triangle_indices));
        return alloc_vtc;
    }
};

struct ObjVtxDat {
    const std::vector<vec3> static_vtc;
    range alloc_vtc;
    range alloc_tris;

    explicit ObjVtxDat(const std::vector<vec3> &vertices,
                       const range &alloc_vtc,
                       const range &alloc_tris)
        : static_vtc(vertices), alloc_vtc(alloc_vtc), alloc_tris(alloc_tris) {
    }
};

class BaseObj {
protected:
    CMRefFrame main_frame; // Center of mass
    ObjVtxDat vtx_dat;

    void updateWorldVtc(GlobalVertices &global_vtc) const {
        // Update world_vertices[vtx] based on main_frame attributes
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; v++)
            global_vtc.world_vtc[v].pos = main_frame.pos + vtx_dat.static_vtc[v];
    }



public:
    explicit BaseObj(const std::vector<vec3> &static_vertices,
                     const range &alloc_vtc,
                     const range &alloc_tris,
                     std::vector<VtxRefFrame> &world_vertices)
        : vtx_dat(static_vertices, alloc_vtc, alloc_tris) {
        for (auto v = alloc_vtc.first; v < alloc_vtc.second; ++v)
            world_vertices[v].owner = this;
    }

    virtual ~BaseObj() = default;

    void printDat(const GlobalVertices &global_vtc) const {
        std::cout << "Vertices (world):\n";
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v)
            std::cout << global_vtc.world_vtc[v].pos << '\n';

        std::cout << "Vertices (optical):\n";
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v)
            std::cout << global_vtc.optical_vtc[v] << '\n';
        std::cout << '\n';
    }

    void process(const double dt, const CamRefFrame &cam, GlobalVertices &global_vtc) {
        main_frame.process(dt);
        updateWorldVtc(global_vtc);

        if (time_since_last_frame != 0.0) return;
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v) {
            global_vtc.world_vtc[v].saveState();
            const double t_ret = global_vtc.world_vtc[v].tRet(cam);
            global_vtc.optical_vtc[v] = global_vtc.world_vtc[v].getOptPos(cam, t_ret);
        }
    }

    void offset(const vec3 &dx, GlobalVertices &global_vtc) {
        main_frame.pos += dx;
        updateWorldVtc(global_vtc);
    }

    void resetHistory(GlobalVertices &global_vtc) const {
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v)
            global_vtc.world_vtc[v].resetHistory();
    }
};

#endif
