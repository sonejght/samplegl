/* Ⓒ 2023 - Sonejght */
#pragma once
#include <son8/window.hxx>
#include "cube.hxx"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

static auto RotateX = 45.f;
static auto RotateY = 45.f;

namespace son8::helper
{
    template< typename Draw >
    class DrawBase
    {
        void crtp_create() { (static_cast< Draw* >(this))->data_create(); }
        void crtp_accept() { (static_cast< Draw* >(this))->data_accept(); }
        void crtp_render() { (static_cast< Draw* >(this))->data_render(); }
        void crtp_reject() { (static_cast< Draw* >(this))->data_reject(); }
        void crtp_delete() { (static_cast< Draw* >(this))->data_delete(); }
    public:

        DrawBase() { crtp_create(); }
        ~DrawBase() { crtp_delete(); }
        void operator ()() { crtp_accept(); crtp_render(); crtp_reject(); }
    };

    template< typename Draw >
    Draw& init()
    {
        son8::window_init(640, 480);
        static Draw draw;
        son8::call([](auto event){
            if (event.action == GLFW_REPEAT || event.action == GLFW_PRESS) {
                switch (event.key) {
                    case GLFW_KEY_LEFT: RotateY += 15; break;
                    case GLFW_KEY_RIGHT: RotateY -= 15; break;
                    case GLFW_KEY_DOWN: RotateX -= 15; break;
                    case GLFW_KEY_UP: RotateX += 15; break;
                    default: break;
                }
            }
        });

        return draw;
    }
}
