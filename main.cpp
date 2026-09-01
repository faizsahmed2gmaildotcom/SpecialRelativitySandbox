#include <iostream>
#include <chrono>
#include "vector_matrix.h"
#include "objects.h"
#include "relativity.h"
#include "config.h"
using Clock = std::chrono::steady_clock;


int main() {
    CamRefFrame cam{{}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    BaseObj<3> test_obj{
        {
            {
                Vector<double, 3>{0.0, 10.0, 0.0},
                Vector<double, 3>{0.0, 0.0, 0.0},
                Vector<double, 3>{0.0, -10.0, 0.0}
            }
        }
    };
    test_obj.offset({0.0, 0.0, 100.0});
    test_obj.resetHistory();

    double total_t = 0.0;
    int total_physics_frames = 0;
    auto cur_time = Clock::now();
    while (total_t < 2.0) {
        if (total_t >= 1.0) cam.setVelocity({0.0, 0.0, 6.7});

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
    std::cout << "Average PPS: " << total_physics_frames / total_t << '\n';  // Physics Per Second

    return 0;
}
