#pragma once
#include <Core/Logger/Log.h>

namespace Render {

    enum class EShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static Uint ShaderDataTypeSize(EShaderDataType type)
    {
        static constexpr Uint size = 4;
        switch(type)
        {
        case EShaderDataType::Float: return size;
        case EShaderDataType::Float2: return size * 2;
        case EShaderDataType::Float3: return size * 3;
        case EShaderDataType::Float4: return size * 4;
        case EShaderDataType::Mat3: return size * 3 * 3;
        case EShaderDataType::Mat4: return size * 4 * 4;
        case EShaderDataType::Int: return size;
        case EShaderDataType::Int2: return size * 2;
        case EShaderDataType::Int3: return size * 3;
        case EShaderDataType::Int4: return size * 4;
        case EShaderDataType::Bool: return 1;
        }

        LOG_FATAL("Unknown EShaderDataType!");
        return 0;
    }

    struct BufferAttribute
    {
        String Name;
        EShaderDataType Type;
        Uint Size;
        size_t Offset;
        bool Normalized;

        BufferAttribute() = default;

        BufferAttribute(EShaderDataType type, String name, bool normalized = false)
            : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        Uint8 GetComponentCount() const
        {
            switch(Type)
            {
            case EShaderDataType::Float: return 1;
            case EShaderDataType::Float2: return 2;
            case EShaderDataType::Float3: return 3;
            case EShaderDataType::Float4: return 4;
            case EShaderDataType::Mat3: return 3;
            case EShaderDataType::Mat4: return 4;
            case EShaderDataType::Int: return 1;
            case EShaderDataType::Int2: return 2;
            case EShaderDataType::Int3: return 3;
            case EShaderDataType::Int4: return 4;
            case EShaderDataType::Bool: return 1;
            case EShaderDataType::None:
            default: LOG_ERROR("Unknown EShaderDataType!");
            }
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferAttribute> elements) : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        Uint GetStride() const { return m_Stride; }
        const TVector<BufferAttribute>& GetElements() const { return m_Elements; }

        TVector<BufferAttribute>::iterator begin() { return m_Elements.begin(); }
        TVector<BufferAttribute>::iterator end() { return m_Elements.end(); }
        TVector<BufferAttribute>::const_iterator begin() const { return m_Elements.begin(); }
        TVector<BufferAttribute>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride      = 0;
            for(auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        TVector<BufferAttribute> m_Elements;
        Uint m_Stride = 0;
    };

    /** @class VertexBuffer
     *
     * @brief Wrapper for Vertex buffer object
     */
    class VertexBuffer
    {
    public:
        VertexBuffer(Uint size);
        VertexBuffer(float* vertices, Uint size);
        VertexBuffer(FVector& vertices);
        VertexBuffer(TVector<Vertex>& vertices, BufferLayout& layout);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, Uint size);

        [[nodiscard]] const BufferLayout& GetLayout() const;
        void SetLayout(const BufferLayout& layout);

    private:
        Uint m_ID;
        BufferLayout m_Layout;
    };

    /** @class ElementBuffer
     *
     * @brief Wrapper for Element buffer object
     */
    class ElementBuffer
    {
    public:
        ElementBuffer(Uint count);
        ElementBuffer(Uint* indices, Uint count);
        ElementBuffer(TVector<Uint>& indices);
        ~ElementBuffer();

        void Bind() const;
        void Unbind() const;

        [[nodiscard]] Uint GetCount() const;

    private:
        Uint m_ID;
        Uint m_Count;
    };
} // namespace Render
