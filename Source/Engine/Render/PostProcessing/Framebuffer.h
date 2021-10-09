#pragma once

namespace Render {
    class Shader;

    class Framebuffer
    {
    public:
        ~Framebuffer();

        bool Init(Shader* _shader);

        void BindSceneBegin() const;
        void BindSceneEnd() const;

    private:
        Shader* shader = nullptr;


        unsigned m_VAO              = 0;
        unsigned m_VBO              = 0;
        unsigned m_RBO              = 0;
        unsigned nativeFb           = 0;
        unsigned textureColorBuffer = 0;
    };
} // namespace Render
