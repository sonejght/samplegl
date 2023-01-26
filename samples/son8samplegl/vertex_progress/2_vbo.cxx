/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_5.hxx>
#include "helper/header.hxx"
#include <iostream>
#include <algorithm>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

constexpr auto VBO_SIZE = 1;
constexpr auto VBO_ARRAY_INDEX = 0;
GLuint vbo[VBO_SIZE];

using Elements = gl::types::Elements< std::vector< unsigned > >;
Elements *Elems;

class Draw : public son8::helper::DrawBase< Draw >
{
    using Cube = son8::helper::Cube;
public:
    void data_create()
    {
        static std::vector< unsigned > vec(Cube::IndicesCount);
        int i = 0;
        std::generate(vec.begin(), vec.end(), [&i](){ return Cube::model.indices[i++]; });
        Elems = new Elements(vec, GL::Draw::Strip);
        glGenBuffers(VBO_SIZE, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_ARRAY_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, Cube::Stride * Cube::VerticesCount, Cube::model.vertices, GL_STATIC_DRAW);
        gl::GenBuffers(*Elems);
        gl::BindBuffer(*Elems);
        gl::BufferData(*Elems);
        data_reject();
    }

    void data_delete()
    {
        glDeleteBuffers(VBO_SIZE, vbo);
        gl::DeleteBuffers(*Elems);
        delete Elems;
    }

    void data_accept()
    {
        gl::EnableClientState(GL::Array::Vertex);
        gl::EnableClientState(GL::Array::Color);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_ARRAY_INDEX]);
        gl::BindBuffer(*Elems);

        glVertexPointer(3, GL_FLOAT, Cube::Stride, (void *)0);
        glColorPointer(3, GL_FLOAT, Cube::Stride, (void *)Cube::ColOffset);
    }
    void data_reject()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
