#pragma once

namespace Render {
    class Shader;

    class ShaderManager
    {
    public:
        ShaderManager(const ShaderManager&) = delete;
        ShaderManager(ShaderManager&&)      = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;

        bool AddShader(const StringView& name);
        Shader* GetShader(const String& name) const;

    private:
        std::unordered_map<StringView, std::unique_ptr<Shader>> m_Table;
        inline static String directory{"Resources/Shaders/"};

        ShaderManager() = default;
        bool m_LoadDefaultShaders();

        friend Renderer;
    };
} // namespace Render
