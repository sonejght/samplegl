#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

int main()
{
    assert(glfwInit() == GLFW_TRUE);
    glfwSetErrorCallback([](auto error, auto msg) {
        std::cerr << error << ' ' << msg << '\n';
    });

    auto window = glfwCreateWindow(640, 480, "glfw-min", nullptr, nullptr);
    assert(window != nullptr);

    glfwSetKeyCallback(window, [](auto window, auto key, auto scancode, auto action, auto mods) {
        if (key ==GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
