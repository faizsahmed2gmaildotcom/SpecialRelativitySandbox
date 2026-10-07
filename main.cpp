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
    CamRefFrame cam{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    BaseObj test_obj{
        {
            vec3{0.0f, 0.0f, 0.0f},
            vec3{-10.0f, -10.0f, 0.0f},
            vec3{10.0f, -10.0f, 0.0f}
        },
        global_dat.allocVtc(3, tris({0u, 1u, 2u})),
        global_dat.allocJoints(tris({0u, 1u, 2u})),
        global_dat.allocCollTris(tris({0u, 1u, 2u})),
        global_dat
    };
    test_obj.offset({0.0f, 0.0f, 100.0f}, global_dat);

    float total_t = 0.0;
    int total_physics_frames = 0;
    auto cur_time = Clock::now();

    const auto start_time = cur_time;
    while (total_t < 2.0) {
        if (total_t > 1.0) cam.setVelocity({0.0f, 0.0f, 10.0f});  // FOR TESTING ONLY

        auto new_time = Clock::now();
        float dt = std::chrono::duration<float>(new_time - cur_time).count();
        if (dt < 1e-6) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            new_time += std::chrono::microseconds(1);
            dt += 1e-6;
        }
        cur_time = new_time;
        time_since_last_frame += dt;

        if (time_since_last_frame >= spf) {
            time_since_last_frame = 0.0;
            // Render OpenGL frame via custom render(global_dat) function
        }

        cam.process(dt);
        test_obj.process(dt, cam, global_dat);
        total_t += dt;
        total_physics_frames++;
    }
    const auto end_time = Clock::now();

    test_obj.printDat(global_dat);
    std::cout << "Cam pos (world): " << cam.pos << '\n';
    std::cout << "Total time: " << total_t << "s\n";
    std::cout << "Actual time: " << std::chrono::duration<float>(end_time - start_time) << '\n';
    std::cout << "Average PPS: " << static_cast<float>(total_physics_frames) / total_t << '\n'; // Physics Per Second

    return 0;
}
