#include <precomp.h>
#include "Buffer.h"

Render::VertexBuffer::VertexBuffer(Uint size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

Render::VertexBuffer::VertexBuffer(float* vertices, Uint size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
}

Render::VertexBuffer::VertexBuffer(FVector& vertices)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

Render::VertexBuffer::VertexBuffer(TVector<Vertex>& vertices, BufferLayout& layout) : m_Layout(layout)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

Render::VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_ID); }

void Render::VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_ID); }

void Render::VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void Render::VertexBuffer::SetData(const void* data, Uint size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const Render::BufferLayout& Render::VertexBuffer::GetLayout() const { return m_Layout; }

void Render::VertexBuffer::SetLayout(const BufferLayout& layout) { m_Layout = layout; }

Render::ElementBuffer::ElementBuffer(Uint count) : m_ID(-1), m_Count(count) {}

Render::ElementBuffer::ElementBuffer(Uint* indices, Uint count) : m_Count(count)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(Uint), indices, GL_STATIC_DRAW);
}

Render::ElementBuffer::ElementBuffer(TVector<Uint>& indices) : m_Count(indices.size())
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(Uint), indices.data(), GL_STATIC_DRAW);
}

Render::ElementBuffer::~ElementBuffer() { glDeleteBuffers(1, &m_ID); }

void Render::ElementBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); }

void Render::ElementBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

Uint Render::ElementBuffer::GetCount() const { return m_Count; }
