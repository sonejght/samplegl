#define GLFW_INCLUDE_NONE
//#include <glad/gl.h>
#include <son8/opengl.hxx>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

int main()
{
    assert(glfwInit() == GLFW_TRUE);
    glfwSetErrorCallback([](auto error, auto msg) {
        std::cerr << error << ' ' << msg << '\n';
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    auto window = glfwCreateWindow(640, 480, "glfw-mingl", nullptr, nullptr);
    assert(window != nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGL((GLADloadfunc)glfwGetProcAddress);

    glfwSetKeyCallback(window, [](auto window, auto key, auto scancode, auto action, auto mods) {
        if (key ==GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    glfwSwapInterval(1);
    GLfloat rgb[3] = {0.5, 0.25, 0.0};

    while (!glfwWindowShouldClose(window)) {
        auto timesec = glfwGetTime();
        auto slowness = 1 / (timesec * 0.125);
        auto fading = 1 * slowness;
        assert(glGetError() == GL_NO_ERROR);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(fading + rgb[0], fading + rgb[1], fading + rgb[2], 0.f);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
