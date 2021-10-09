#pragma once

namespace Render {
    class Mesh
    {
    public:
        Mesh() = default;

        void AddVertexArray(TVector<Vertex>& vertices, TVector<Uint>& indices);
        VertexArray& GetVertexArray();

    private:
        VertexArray m_VertexArray{};
    };
} // namespace Render
