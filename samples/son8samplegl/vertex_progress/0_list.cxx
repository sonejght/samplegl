/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_1.hxx>
#include "helper/header.hxx"
#include <iostream>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;

class Draw : public son8::helper::DrawBase< Draw >
{
    using list_type = decltype(glGenLists(0));
    list_type list;
public:
    void data_create()
    {
        std::cout << "create" << '\n';
        using v = gl::types::array3f;
        using c = gl::types::array3f;
        using cube = son8::helper::Cube;
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        gl::Begin(GL::Draw::Strip);
        {
            for (auto i : cube::model.indices) {
                const auto& vertex = cube::model.vertices[i];
                gl::Color(c{vertex.col[0], vertex.col[1], vertex.col[2]});
                gl::Vertex(v{vertex.pos[0], vertex.pos[1], vertex.pos[2]});
            }
        }
        gl::End();
        glEndList();
    }

    void data_delete()
    {
        std::cout << "delete" << '\n';
        glDeleteLists(list, 1);
    }

    void data_accept() { }
    void data_reject() { }

    void data_render()
    {
        glCallList(list);
    }
};

int main()
{
    static auto &draw = son8::helper::init< Draw >();

    gl::Enable(GL::Capability::Depth);
    gl::ClearColor(.2f, .2f, .2f, 1.f);

    son8::run([](){
        gl::Clear();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(RotateX, 1.f, 0.f, 0.f);
        glRotatef(RotateY, 0.f, 1.f, 0.f);
        glScalef(.5f, .5f, .5f);

        draw();
    });
}
