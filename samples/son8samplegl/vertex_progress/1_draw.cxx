/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_1.hxx>
#include "helper/header.hxx"
#include <iostream>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

class Draw : public son8::helper::DrawBase< Draw >
{
    using Cube = son8::helper::Cube;
public:
    void data_create() { std::cout << "create" << '\n'; }

    void data_delete() { std::cout << "delete" << '\n'; }

    void data_accept()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, Cube::Stride, Cube::model.vertices);
        glColorPointer(3, GL_FLOAT, Cube::Stride, (GLubyte*)son8::helper::Cube::model.vertices + Cube::ColOffset);
    }
    void data_reject()
    {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    void data_render()
    {
        glDrawElements(GL_TRIANGLE_STRIP, Cube::IndicesCount, GL_UNSIGNED_BYTE, Cube::model.indices);
    }
};

int main()
{
    static auto &draw = son8::helper::init< Draw >();

    gl::Enable(GL::Capability::Depth);
    gl::ClearColor(.2f, .2f, .2f, 1.f);

    son8::run([](){
        gl::Clear();

        auto mvp = glm::ortho(-1.f, 1.f,-1.f, 1.f,-1.f, 1.f);
        auto modelview = glm::mat4(1.f);
        modelview = glm::rotate(modelview, glm::radians(RotateX), glm::vec3(1.f, 0.f, 0.f));
        modelview = glm::rotate(modelview, glm::radians(RotateY), glm::vec3(0.f, 1.f, 0.f));
        modelview = glm::scale(modelview, glm::vec3(.5f));
        mvp *= modelview;
        glLoadMatrixf(glm::value_ptr(mvp));

        draw();
    });
}
