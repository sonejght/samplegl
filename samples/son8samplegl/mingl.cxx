/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_1.hxx>
#include <son8/window.hxx>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

int main()
{
    son8::window_init(640, 480);

    son8::run([](){
        using v = gl::types::array2f;
        using c = gl::types::array3f;
        gl::Clear();
        gl::ClearColor(.8f, .8f, .2f, 1.f);

        gl::Begin();
        {
            gl::Color(c{.2f, .2f, .8f});
            gl::Vertex(v{-.8f,-.8f});
            gl::Vertex(v{ .8f,-.8f});
            gl::Vertex(v{ .0f, .8f});
        }
        gl::End();
    });
}
