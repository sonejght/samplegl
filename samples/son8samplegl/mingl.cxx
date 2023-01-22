/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_1.hxx>
#include <son8/window.hxx>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

int main()
{
    son8::window_init(640, 480);

    son8::run([](){
        gl::Clear();
        gl::ClearColor(.8f, .8f, .2f, 1.f);
    });
}
