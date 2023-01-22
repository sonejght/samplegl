/* Ⓒ 2023 - Sonejght */

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <son8/opengl-4_3-comp.hxx>
#include <son8/window.hxx>

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

class WindowOpenGL
{
private:
    GLFWwindow* window_;
    std::string title_;
    unsigned width_;
    unsigned height_;
    unsigned long version_;
    bool init_;
public:
    WindowOpenGL(unsigned w, unsigned h, unsigned long v) : width_(w), height_(h), version_(v)
    {
        init_ = glfwInit();
        assert(init_ == GLFW_TRUE);
        glfwSetErrorCallback([](auto err, auto msg) { std::cerr << err << ": " << msg << '\n'; });

        auto context = static_cast< uint8_t >(version_);
        auto minor = static_cast< uint8_t >(version_ >> 8);
        auto major = static_cast< uint8_t >(version_ >> 16);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

        bool core = context == 0xCE;
        if (context) glfwWindowHint(GLFW_OPENGL_PROFILE, (core) ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);

        std::stringstream ss;
        ss << (int)major << (int)minor * 10 << ' ' << ((core) ? "core" : "compatibility");
        title_ = ss.str();

        window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
        assert(window_ != nullptr);

        glfwMakeContextCurrent(window_);
        gladLoadGL(reinterpret_cast< GLADloadfunc >(glfwGetProcAddress));

        glfwSwapInterval(1);
        glfwSetWindowSizeCallback(window_, [](auto window, auto w, auto h) { son8::opengl::Viewport(w, h); });
    }
    ~WindowOpenGL() noexcept
    {
        if (!init_) return;
        if (window_) glfwDestroyWindow(window_);
        glfwTerminate();
    }

    operator GLFWwindow*() { return window_; }
};

static WindowOpenGL *window = nullptr;

namespace son8
{
    void window_init(unsigned w, unsigned h, unsigned long v)
    {
        static WindowOpenGL windowOpenGL(w, h, v);
        if (window == nullptr) window = &windowOpenGL;
    }

    void run(window_run_func func)
    {
        while (!glfwWindowShouldClose(*window))
        {
            func();
            son8::opengl::GetError();
            glfwSwapBuffers(*window);
            glfwPollEvents();
        }
    }
}
