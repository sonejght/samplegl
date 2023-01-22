#include <son8/opengl-1_1.hxx>
#include <son8/window.hxx>

int main()
{
    son8::window_init(640, 480);

    son8::run([](){
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.8, 0.8, 0.2, 1.0);
    });
}
