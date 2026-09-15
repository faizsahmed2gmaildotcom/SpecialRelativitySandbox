#ifndef OBJECTS
#define OBJECTS
#include "vector_matrix.h"
#include "relativity.h"

struct GlobalVertices {
    std::vector<Array<float, 3> > optical_vtc; // float instead of double for GPU rendering
    tris triangle_opt; // Optical tris

    std::vector<VtxRefFrame> world_vtc;
    tris triangle_coll; // Collision tris
    tris triangle_joints; // Soft bodies

    // The first n_world vertices in static_vertices are allocated as optical vertices
    range allocVtc(const unsigned n_world, tris &&opt_tris) {
        const range alloc_world_vtc = {world_vtc.size(), world_vtc.size() + n_world};
        const range alloc_opt_vtc = {optical_vtc.size(), optical_vtc.size() + opt_tris.size() * 3};

        old_world_vtx_size = world_vtc.size();
        world_vtc.resize(world_vtc.size() + n_world);
        optical_vtc.resize(optical_vtc.size() + opt_tris.size() * 3);

        allocTris(opt_tris, triangle_opt);

        for (unsigned v_w = alloc_world_vtc.first, v_o = alloc_opt_vtc.first; v_o < alloc_opt_vtc.second; v_w++, v_o++)
            world_vtc[v_w].optical_idx = v_o;

        return alloc_world_vtc;
    }

    range allocJoints(tris &&new_tris) {
        return allocTris(new_tris, triangle_joints);
    }

    range allocCollTris(tris &&new_tris) {
        return allocTris(new_tris, triangle_coll);
    }

    range allocOptTris(tris &&new_tris) {
        return allocTris(new_tris, triangle_opt);
    }

private:
    unsigned old_world_vtx_size = 0;

    // Assumes allocVtc has been called before this
    range allocTris(tris &new_tris, tris &tris_to_extend) const {
        for (auto &tri: new_tris)
            for (unsigned &vtx: tri)
                vtx += old_world_vtx_size;

        range alloc_tris = {tris_to_extend.size(), tris_to_extend.size() + new_tris.size()};
        tris_to_extend.reserve(tris_to_extend.size() + new_tris.size());
        tris_to_extend.append_range(std::views::as_rvalue(new_tris));
        return alloc_tris;
    }
};

struct ObjVtxDat {
    const std::vector<vec3> static_vtc;
    range alloc_vtc;
    range alloc_joints;
    range alloc_coll;

    explicit ObjVtxDat(std::vector<vec3> &&vertices,
                       range &&alloc_vtc,
                       range &&alloc_joints,
                       range &&alloc_coll)
        : static_vtc(std::move(vertices)), alloc_vtc(std::move(alloc_vtc)), alloc_joints(std::move(alloc_joints)), alloc_coll(std::move(alloc_coll)) {
    }
};

class BaseObj {
protected:
    CMRefFrame main_frame; // Center of mass
    ObjVtxDat vtx_dat;

    void updateWorldVtc(GlobalVertices &global_dat) const {
        // Update world_vertices[vtx] based on main_frame attributes
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; v++)
            global_dat.world_vtc[v].pos = main_frame.pos + vtx_dat.static_vtc[v];
    }

public:
    explicit BaseObj(std::vector<vec3> &&static_vertices,
                     range &&alloc_vtc,
                     range &&alloc_joints,
                     range &&alloc_coll_tris,
                     GlobalVertices &global_dat)
        : vtx_dat(std::move(static_vertices), std::move(alloc_vtc), std::move(alloc_joints), std::move(alloc_coll_tris)) {
        for (auto v = alloc_vtc.first; v < alloc_vtc.second; ++v)
            global_dat.world_vtc[v].owner = this;
    }

    virtual ~BaseObj() = default;

    void printDat(const GlobalVertices &global_dat) const {
        std::cout << "Vertices (world):\n";
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v)
            std::cout << global_dat.world_vtc[v].pos << '\n';

        std::cout << "Vertices (optical):\n";
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v)
            if (const unsigned opt_idx = global_dat.world_vtc[v].optical_idx; opt_idx != -1)
                std::cout << global_dat.optical_vtc[opt_idx] << '\n';

        std::cout << '\n';
    }

    void process(const double dt, const CamRefFrame &cam, GlobalVertices &global_dat) {
        main_frame.process(dt);
        updateWorldVtc(global_dat);

        if (time_since_last_frame != 0.0) return;
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v) {
            VtxRefFrame &world_vtx = global_dat.world_vtc[v];
            world_vtx.saveState();

            if (world_vtx.optical_idx != -1) {
                const double t_ret = world_vtx.tRet(cam);
                global_dat.optical_vtc[world_vtx.optical_idx] = world_vtx.getOptPos(cam, t_ret);
            }
        }
    }

    void offset(const vec3 &dx, GlobalVertices &global_dat) {
        main_frame.pos += dx;
        updateWorldVtc(global_dat);
    }

    void resetHistory(GlobalVertices &global_dat) const {
        for (auto v = vtx_dat.alloc_vtc.first; v < vtx_dat.alloc_vtc.second; ++v)
            global_dat.world_vtc[v].resetHistory();
    }
};

#endif
