/* Ⓒ 2023 - Sonejght */
#pragma once
#include <array>
#include <cstdint>

namespace son8::helper
{
    struct Cube
    {
    public:
        static constexpr auto VerticesCount = 8;
        static constexpr auto IndicesCount = 14;
    private:
        struct Model
        {
            struct Vertex
            {
                using fvec3 = float[3];
                fvec3 pos;
                fvec3 col;
            };
            using vertices_type = Vertex[VerticesCount];
            using indices_type = uint8_t[IndicesCount];
            vertices_type vertices;
            indices_type indices;
        };
    public:
        static constexpr auto Stride = sizeof(Model::Vertex);
        static constexpr auto ColOffset = offsetof(Model::Vertex, col);
        static constexpr Model model = {
            {
                { {+1,+1,+1}, {1, 1, 0} },
                { {-1,+1,+1}, {0, 1, 0} },
                { {+1,+1,-1}, {1, 1, 1} },
                { {-1,+1,-1}, {0, 1, 1} },
                { {+1,-1,+1}, {1, 0, 0} },
                { {-1,-1,+1}, {0, 0, 0} },
                { {-1,-1,-1}, {0, 0, 1} },
                { {+1,-1,-1}, {1, 0, 1} },
            },
            {3,2,6,7,4,2,0,3,1,6,5,4,1,0},
        };
        static constexpr auto IndicesSize = sizeof(model.indices[0]) * IndicesCount;
        static constexpr auto VerticesSize = sizeof(model.vertices[0]) * VerticesCount;
    };
}
