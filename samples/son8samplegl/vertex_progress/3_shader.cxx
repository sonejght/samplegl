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
    gl::types::Program program = {0};
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

        gl::types::Shader vert = gl::CreateShader(GL::Shader::Vertex);
        gl::types::Shader frag = gl::CreateShader(GL::Shader::Fragment);
        gl::ShaderSource(vert, vert_shader_source.c_str());
        gl::ShaderSource(frag, frag_shader_source.c_str());
        gl::CompileShader(vert);
        gl::CompileShader(frag);
        Program.program = gl::CreateProgram();
        gl::AttachShader(Program.program, vert);
        gl::AttachShader(Program.program, frag);
        gl::LinkProgram(Program.program);
        gl::DeleteShader(vert);
        gl::DeleteShader(frag);

        Program.pos = gl::GetAttribLocation(Program.program, "pos");
        Program.col = gl::GetAttribLocation(Program.program, "col");
    }

    void data_delete()
    {
        glDeleteBuffers(VBO_SIZE, vbo);
        gl::DeleteBuffers(*Elems);
        gl::DeleteProgram(Program.program);
        delete Elems;
    }

    void data_accept()
    {
        gl::EnableVertexAttribArray(Program.program);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_ARRAY_INDEX]);
        gl::BindBuffer(*Elems);

        glVertexAttribPointer(Program.pos, 3, GL_FLOAT, false, Cube::Stride, (void *)0);
        glVertexAttribPointer(Program.col, 3, GL_FLOAT, false, Cube::Stride, (void *)Cube::ColOffset);
        gl::UseProgram(Program.program);
    }
    void data_reject()
    {
        gl::UseProgram();
        gl::BindBuffer(GL::Buffer::Array);
        gl::BindBuffer(GL::Buffer::Element);
        gl::DisableVertexAttribArray(Program.program);
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
        gl::LoadMatrix(gl::types::to_array16(glm::value_ptr(mvp)));

        draw();
    });
}
