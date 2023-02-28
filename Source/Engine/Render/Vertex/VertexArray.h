#pragma once
#include "Buffer.h"


namespace Render {
    class VertexArray
    {
    public:
        VertexArray();
        VertexArray(VertexArray& other);
        VertexArray& operator=(VertexArray& other);
        ~VertexArray();
        VertexArray(VertexBuffer* vertexBuffer, ElementBuffer* elementBuffer);

        void Bind() const;
        void Unbind() const;
        void Draw() const;
        void DrawInstanced(int count) const;

        void AddVertexBuffer(VertexBuffer* vertexBuffer);
        void SetElementBuffer(ElementBuffer* elementBuffer);

        TVector<VertexBuffer*> GetVertexBuffers() const;
        const ElementBuffer& GetElementBuffer() const;

    private:
        unsigned m_ID;
        TVector<TUniquePtr<VertexBuffer>> m_VBuffers;
        TUniquePtr<ElementBuffer> m_EBuffer;
    };
} // namespace Render
