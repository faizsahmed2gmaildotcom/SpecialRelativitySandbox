#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

const bool WAYLAND = getenv("WAYLAND_DISPLAY");
constexpr bool VSYNC = false;
constexpr float FPS = 280 * 2000;
constexpr auto MUS_PER_FRAME = std::chrono::microseconds(static_cast<unsigned>(1e6 / FPS));

static GLFWwindow *initializeGLFW() {
    if (WAYLAND)
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    if (VSYNC) glfwSwapInterval(1); // VSync


    GLFWwindow *window = glfwCreateWindow(800, 800, "Special Relativity Sandbox", nullptr, nullptr);
    if (not window) {
        std::cout << "Failed to create window!\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    return window;
}

int main() {
    GLFWwindow *window = initializeGLFW();
    if (not window) return -1;

    if (not gladLoadGL()) {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    unsigned long frame = 0;
    const auto start_time = glfwGetTime();

    while (not glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();

        frame++;
        std::this_thread::sleep_for(MUS_PER_FRAME);
    }

    std::cout << "Average FPS = " << static_cast<double>(frame) / (glfwGetTime() - start_time) << '\n';

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
