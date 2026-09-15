#include <iostream>
#include <chrono>
#include <thread>

#include "debug_tools.h"
#include "vector_matrix.h"
#include "objects.h"
#include "collision.h"
#include "relativity.h"
#include "config.h"
using Clock = std::chrono::steady_clock;


int main() {
    GlobalVertices global_dat;
    CamRefFrame cam{{}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    BaseObj test_obj{
        {
            vec3{0.0, 10.0, 0.0},
            vec3{-10.0, -10.0, 0.0},
            vec3{10.0, -10.0, 0.0}
        },
        global_dat.allocVtc(3, tris({0u, 1u, 2u})),
        global_dat.allocJoints(tris({0u, 1u, 2u})),
        global_dat.allocCollTris(tris({0u, 1u, 2u})),
        global_dat
    };
    test_obj.offset({0.0, 0.0, 100.0}, global_dat);
    test_obj.resetHistory(global_dat);

    double total_t = 0.0;
    int total_physics_frames = 0;
    auto cur_time = Clock::now();

    const auto start_time = cur_time;
    while (total_t < 2.0) {
        if (total_t >= 1.0) cam.setVelocity({0.0, 0.0, 10.0});

        auto new_time = Clock::now();
        const double dt = std::chrono::duration<double>(new_time - cur_time).count();
        cur_time = new_time;
        time_since_last_frame += dt;
        if (time_since_last_frame >= spf) time_since_last_frame = 0.0;

        cam.process(dt);
        test_obj.process(dt, cam, global_dat);
        total_t += dt;
        total_physics_frames++;
    }
    const auto end_time = Clock::now();

    test_obj.printDat(global_dat);
    std::cout << "Cam pos (world): " << cam.pos << '\n';
    std::cout << "Total time: " << total_t << "s\n";
    std::cout << "Actual time: " << std::chrono::duration<double>(end_time - start_time) << "\n";
    std::cout << "Average PPS: " << total_physics_frames / total_t << '\n'; // Physics Per Second

    return 0;
}
