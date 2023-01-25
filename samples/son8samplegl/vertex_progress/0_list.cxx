/* Ⓒ 2023 - Sonejght */
#include <son8/opengl-1_1.hxx>
#include "helper/header.hxx"
#include <iostream>

namespace gl = son8::opengl;
namespace GL = son8::opengl::enums;
gl::types::vectorList list;

struct Draw : public son8::helper::DrawBase< Draw >
{
    void data_create()
    {
        std::cout << "create" << '\n';
        using v = gl::types::array3f;
        using c = gl::types::array3f;
        using cube = son8::helper::Cube;
        list = gl::GenLists(1);
        std::cout << list[0] << "list\n";
        gl::NewList(list[0]);
        gl::Begin(GL::Draw::Strip);
        {
            for (auto i : cube::model.indices) {
                const auto& vertex = cube::model.vertices[i];
                gl::Color(c{vertex.col[0], vertex.col[1], vertex.col[2]});
                gl::Vertex(v{vertex.pos[0], vertex.pos[1], vertex.pos[2]});
            }
        }
        gl::End();
        gl::EndList();
    }

    void data_delete()
    {
        std::cout << "delete" << '\n';
        gl::DeleteLists(list);
    }

    void data_accept() { }
    void data_reject() { }

    void data_render()
    {
        std::cout << "lsit size: " << list.size() << std::endl;
        gl::CallList(list[0]);
    }
};

int main()
{
    static auto &draw = son8::helper::init< Draw >();

    gl::Enable(GL::Capability::Depth);
    gl::ClearColor(.2f, .2f, .2f, 1.f);

    son8::run([](){
        gl::Clear();

        gl::MatrixMode(GL::Matrix::Projection);
        gl::LoadIdentity();
        gl::Ortho(-1, 1, -1, 1, -1, 1);

        gl::MatrixMode(GL::Matrix::ModelView);
        gl::LoadIdentity();
        gl::Rotate(RotateX, 1.f, 0.f, 0.f);
        gl::Rotate(RotateY, 0.f, 1.f, 0.f);
        gl::Scale(.5f, .5f, .5f);

        draw();
    });
}
