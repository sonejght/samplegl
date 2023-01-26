/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-2_1.hxx>
#include "helper/header.hxx"
#include <iostream>
#include <algorithm>
#include <string>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

constexpr auto VBO_SIZE = 1;
constexpr auto VBO_ARRAY_INDEX = 0;
GLuint vbo[VBO_SIZE];

std::string vert_shader_source = R"GLSL(
#version 120
attribute vec4 pos;
attribute vec4 col;
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * pos;
    gl_FrontColor = col;
}
)GLSL";
std::string frag_shader_source = R"GLSL(
#version 120
void main()
{
    gl_FragColor = gl_Color;
}
)GLSL";
struct GLSLProgram
{
    GLint pos;
    GLint col;
    GLuint index;
};

GLSLProgram Program;

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
        vec.clear();

        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        std::string_view s = vert_shader_source;
        const char *vert_source = vert_shader_source.c_str();
        const char *frag_source = frag_shader_source.c_str();
        glShaderSource(vert, 1, &vert_source, nullptr);
        glShaderSource(frag, 1, &frag_source, nullptr);
        glCompileShader(vert);
        glCompileShader(frag);
        Program.index = glCreateProgram();
        glAttachShader(Program.index, vert);
        glAttachShader(Program.index, frag);
        glLinkProgram(Program.index);
        glDeleteShader(vert);
        glDeleteShader(frag);

        Program.pos = glGetAttribLocation(Program.index, "pos");
        assert(Program.pos != -1);
        Program.col = glGetAttribLocation(Program.index, "col");
        assert(Program.col != -1);
    }

    void data_delete()
    {
        glDeleteBuffers(VBO_SIZE, vbo);
        gl::DeleteBuffers(*Elems);
        glDeleteProgram(Program.index);
        delete Elems;
    }

    void data_accept()
    {
        glEnableVertexAttribArray(Program.pos);
        glEnableVertexAttribArray(Program.col);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_ARRAY_INDEX]);
        gl::BindBuffer(*Elems);

        glVertexAttribPointer(Program.pos, 3, GL_FLOAT, false, Cube::Stride, (void *)0);
        glVertexAttribPointer(Program.col, 3, GL_FLOAT, false, Cube::Stride, (void *)Cube::ColOffset);
        glUseProgram(Program.index);
    }
    void data_reject()
    {
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(Program.pos);
        glDisableVertexAttribArray(Program.col);
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
