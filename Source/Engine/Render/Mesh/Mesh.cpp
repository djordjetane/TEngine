#include <precomp.h>
#include "Mesh.h"
#include <Vertex/VertexArray.h>

void Render::Mesh::AddVertexArray(TVector<Vertex>& vertices, TVector<Uint>& indices)
{
    BufferLayout layout{{EShaderDataType::Float3, "aPosition"}, {EShaderDataType::Float3, "aNormal"},
        {EShaderDataType::Float2, "aTexCoords"}};
    VertexBuffer* buffer = new VertexBuffer(vertices, layout);
    buffer->SetLayout(layout);
    ElementBuffer* element = new ElementBuffer(indices);

    m_VertexArray.AddVertexBuffer(buffer);
    m_VertexArray.SetElementBuffer(element);
}

Render::VertexArray& Render::Mesh::GetVertexArray() { return m_VertexArray; }
