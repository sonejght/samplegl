/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_1.hxx>
#include "helper/header.hxx"
#include <iostream>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

gl::types::Elements< std::vector< GLubyte > > *Elems;

class Draw : public son8::helper::DrawBase< Draw >
{
    using Cube = son8::helper::Cube;
public:
    void data_create()
    {
        static std::vector< GLubyte > vec(Cube::IndicesCount);
        int i = 0;
        for (auto &v : vec) v = Cube::model.indices[i++];
        Elems = new gl::types::Elements(vec, GL::Draw::Strip);
    }

    void data_delete()
    {
        delete Elems;
    }

    void data_accept()
    {
        gl::EnableClientState(GL::Array::Vertex);
        gl::EnableClientState(GL::Array::Color);

        glVertexPointer(3, GL_FLOAT, Cube::Stride, Cube::model.vertices);
        glColorPointer(3, GL_FLOAT, Cube::Stride, (GLubyte*)son8::helper::Cube::model.vertices + Cube::ColOffset);
    }
    void data_reject()
    {
        gl::DisableClientState(GL::Array::Vertex);
        gl::DisableClientState(GL::Array::Color);
    }

    void data_render()
    {
        gl::DrawElements(*Elems);
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
