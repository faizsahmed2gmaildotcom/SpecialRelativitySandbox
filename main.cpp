#include <iostream>
#include <chrono>
#include "vector_matrix.h"
#include "objects.h"
#include "relativity.h"
using Clock = std::chrono::steady_clock;
constexpr int fps = 280;

void updateCam(const double dt, RefFrame &cam) {
    cam.vel += cam.acc * dt;
    // cam.vel *= 1.0 / std::sqrt(1 + cam.vel.magnitude2() / C2);
    cam.pos += cam.vel * dt * getLF(cam.vel);
}

int main() {
    RefFrame cam;
    cam.vel = {0.0, 0.0, 0.995};
    BaseObj<3> test_obj{
        {
            {
                Vector<double, 3>{0.0, 10.0, 0.0},
                Vector<double, 3>{0.0, 0.0, 0.0},
                Vector<double, 3>{0.0, -10.0, 0.0}
            }
        }
    };
    test_obj.offset({0.0, 0.0, 10.0});

    double total_t = 0.0;
    int total_frames = 0;
    auto cur_time = Clock::now();
    for (int _ = 0; _ < 1e5; _++) {
        auto new_time = Clock::now();
        const double dt = std::chrono::duration<double>(new_time - cur_time).count();
        cur_time = new_time;

        updateCam(dt, cam);
        test_obj.process(dt, cam);
        total_t += dt;
        total_frames++;
    }
    std::cout << test_obj;
    std::cout << "Cam pos (world): " << cam.pos << '\n';
    std::cout << "Total time: " << total_t << "s\n";
    std::cout << "Average PPS: " << total_frames / total_t << '\n';  // Physics Per Second

    return 0;
}
