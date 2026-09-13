#include <iostream>
#include <chrono>
#include "vector_matrix.h"
#include "objects.h"
#include "relativity.h"
#include "config.h"
using Clock = std::chrono::steady_clock;


struct GlobalVertices {
    std::vector<vec3> optical_vertices;
    std::vector<WorldVtx> world_vertices;

    std::vector<vec3 *> allocOptVtc(const int n) {
        std::vector<vec3 *> alloc_vtc;
        alloc_vtc.reserve(n);

        for (int i = 0; i < optical_vertices.size(); i++) {
            if (not world_vertices[i].owner) {
                optical_vertices[i] = {};
                alloc_vtc.push_back(&optical_vertices[i]);
            }
            if (alloc_vtc.size() == n) return alloc_vtc;
        }

        const int old_size = optical_vertices.size();
        optical_vertices.resize(optical_vertices.size() + n - alloc_vtc.size());
        for (int i = old_size; i < optical_vertices.size(); i++)
            alloc_vtc.push_back(&optical_vertices[i]);
        return alloc_vtc;
    }

    std::vector<WorldVtx *> allocWorldVtc(const int n) {
        std::vector<WorldVtx *> alloc_vtc;
        alloc_vtc.reserve(n);

        world_vertices.resize(optical_vertices.size());
        for (auto &world_vtx: world_vertices) {
            if (not world_vtx.owner) {
                world_vtx.pos = {};
                alloc_vtc.push_back(&world_vtx);
                if (alloc_vtc.size() == n) break;
            }
        }
        return alloc_vtc;
    }
};

int main() {
    GlobalVertices global_vtc;

    CamRefFrame cam{{}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    BaseObj test_obj{
        {
            vec3{0.0, 10.0, 0.0},
            vec3{0.0, 0.0, 0.0},
            vec3{0.0, -10.0, 0.0}
        },
        global_vtc.allocOptVtc(3),
        global_vtc.allocWorldVtc(3)
    };
    test_obj.offset({0.0, 0.0, 100.0});
    test_obj.resetHistory();

    double total_t = 0.0;
    int total_physics_frames = 0;
    auto cur_time = Clock::now();
    while (total_t < 10.0) {
        if (total_t >= 1.0) cam.setVelocity({0.0, 0.0, 9.95});

        auto new_time = Clock::now();
        const double dt = std::chrono::duration<double>(new_time - cur_time).count();
        cur_time = new_time;
        time_since_last_frame += dt;
        if (time_since_last_frame >= spf) time_since_last_frame = 0.0;

        cam.process(dt);
        test_obj.process(dt, cam);
        total_t += dt;
        total_physics_frames++;
    }
    std::cout << test_obj;
    std::cout << "Cam pos (world): " << cam.pos << '\n';
    std::cout << "Total time: " << total_t << "s\n";
    std::cout << "Average PPS: " << total_physics_frames / total_t << '\n'; // Physics Per Second

    return 0;
}
