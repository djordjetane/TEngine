#pragma once
#include "Shader.h"

namespace Render {
    class ShaderManager
    {
    public:
        ShaderManager()                     = default;
        ShaderManager(const ShaderManager&) = delete;
        ShaderManager(ShaderManager&&)      = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;

        bool AddShader(String& name);
        Shader* GetShader(const String& name);

    private:
        std::unordered_map<String, std::unique_ptr<Shader>> m_Table;
        inline static String directory{"Resources/Shaders/"};
    };
} // namespace Render
