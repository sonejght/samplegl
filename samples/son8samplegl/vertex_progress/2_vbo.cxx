/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_5.hxx>
#include "helper/header.hxx"
#include <iostream>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

constexpr auto VBO_SIZE = 2;
constexpr auto VBO_ARRAY_INDEX = 0;
constexpr auto VBO_ELEMENT_INDEX = 1;
GLuint vbo[VBO_SIZE];

class Draw : public son8::helper::DrawBase< Draw >
{
    using Cube = son8::helper::Cube;
public:
    void data_create()
    {
        glGenBuffers(VBO_SIZE, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_ARRAY_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, Cube::Stride * Cube::VerticesCount, Cube::model.vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[VBO_ELEMENT_INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Cube::IndicesSize, Cube::model.indices, GL_STATIC_DRAW);
        data_reject();
    }

    void data_delete()
    {
        glDeleteBuffers(VBO_SIZE, vbo);
    }

    void data_accept()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_ARRAY_INDEX]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[VBO_ELEMENT_INDEX]);

        glVertexPointer(3, GL_FLOAT, Cube::Stride, (void *)0);
        glColorPointer(3, GL_FLOAT, Cube::Stride, (void *)Cube::ColOffset);
    }
    void data_reject()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    void data_render()
    {
        glDrawElements(GL_TRIANGLE_STRIP, Cube::IndicesCount, GL_UNSIGNED_BYTE, (void *)0);
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
