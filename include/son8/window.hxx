/* Ⓒ 2023 - Sonejght */
#pragma once
#define GLFW_IMCLUDE_NONE
#include <GLFW/glfw3.h>
namespace son8
{
    struct WindowEvent
    {
        int key;
        int scancode;
        int action;
        int mods;
    };
    using window_run_func = void (*)();
    using window_call_func = void (*)(WindowEvent event);

    inline void window_init(unsigned width, unsigned height)
    {
        extern void window_init(unsigned w, unsigned h, unsigned long version);
#ifdef SON8OPENGL_VERSION
        window_init(width, height, SON8OPENGL_VERSION);
#else
#error "son8 opengl header file should be included before this header (son8/window.hxx)"
#endif
    }

    void run(window_run_func func);
    void call(window_call_func func);
}
