#include <precomp.h>
#include "VertexArray.h"

namespace Render {

    static GLenum ShaderDataTypeToAPIBaseType(EShaderDataType type)
    {
        switch(type)
        {
        case EShaderDataType::Float: return GL_FLOAT;
        case EShaderDataType::Float2: return GL_FLOAT;
        case EShaderDataType::Float3: return GL_FLOAT;
        case EShaderDataType::Float4: return GL_FLOAT;
        case EShaderDataType::Mat3: return GL_FLOAT;
        case EShaderDataType::Mat4: return GL_FLOAT;
        case EShaderDataType::Int: return GL_INT;
        case EShaderDataType::Int2: return GL_INT;
        case EShaderDataType::Int3: return GL_INT;
        case EShaderDataType::Int4: return GL_INT;
        case EShaderDataType::Bool: return GL_BOOL;
        default: LOG_FATAL("Unknown EShaderDataType!");
        }

        return 0;
    }

    VertexArray::VertexArray() { glGenVertexArrays(1, &m_ID); }

    VertexArray::VertexArray(VertexArray& other)
        : m_ID(other.m_ID), m_VBuffers(std::move(other.m_VBuffers)), m_EBuffer(std::move(other.m_EBuffer))
    {
    }

    VertexArray& VertexArray::operator=(VertexArray& other)
    {
        m_ID       = other.m_ID;
        m_VBuffers = std::move(other.m_VBuffers);
        m_EBuffer  = std::move(other.m_EBuffer);
        return *this;
    }

    VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_ID); }

    VertexArray::VertexArray(VertexBuffer* vertexBuffer, ElementBuffer* elementBuffer)
    {
        AddVertexBuffer(vertexBuffer);
        SetElementBuffer(elementBuffer);
    }

    void VertexArray::Bind() const { glBindVertexArray(m_ID); }

    void VertexArray::Unbind() const { glBindVertexArray(0); }

    void VertexArray::Draw() const { glDrawArrays(GL_TRIANGLES, 0, m_EBuffer->GetCount()); }

    void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
    {
        EASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_ID);
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();

        Uint attribCnt = 0;
        for(const auto& element : layout)
        {
            glEnableVertexAttribArray(attribCnt);
            switch(element.Type)
            {
            case EShaderDataType::Float:
            case EShaderDataType::Float2:
            case EShaderDataType::Float3:
            case EShaderDataType::Float4:
            {
                // glVertexAttribPointer(attribCnt, element.GetComponentCount(),
                // ShaderDataTypeToAPIBaseType(element.Type),
                glVertexAttribPointer(attribCnt, element.GetComponentCount(), GL_FLOAT,
                                      element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                                      reinterpret_cast<void*>(element.Offset));
                break;
            }
            case EShaderDataType::Int:
            case EShaderDataType::Int2:
            case EShaderDataType::Int3:
            case EShaderDataType::Int4:
            case EShaderDataType::Bool:
            {
                glVertexAttribIPointer(attribCnt, element.GetComponentCount(),
                                       ShaderDataTypeToAPIBaseType(element.Type), layout.GetStride(),
                                       reinterpret_cast<void*>(element.Offset));
                break;
            }

            case EShaderDataType::Mat3:
            case EShaderDataType::Mat4:
            {
                Uint8 count = element.GetComponentCount();
                for(Uint8 i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(attribCnt);
                    glVertexAttribPointer(attribCnt, count, ShaderDataTypeToAPIBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                                          reinterpret_cast<const void*>(element.Offset + sizeof(float) * count * i));
                    glVertexAttribDivisor(attribCnt, 1);
                    attribCnt++;
                }
                break;
            }

            default: LOG_ERROR("Unknown EShaderDataType!");
            }
            attribCnt++;
        }

        m_VBuffers.emplace_back(vertexBuffer);
    }

    void VertexArray::SetElementBuffer(ElementBuffer* elementBuffer)
    {
        glBindVertexArray(m_ID);
        elementBuffer->Bind();

        m_EBuffer.reset(elementBuffer);
    }

    TVector<VertexBuffer*> VertexArray::GetVertexBuffers() const
    {
        TVector<VertexBuffer*> result;
        std::transform(m_VBuffers.begin(), m_VBuffers.end(), result.begin(), [](auto& el) { return el.get(); });
        return result;
    }

    const ElementBuffer& VertexArray::GetElementBuffer() const { return *m_EBuffer; }

} // namespace Render
