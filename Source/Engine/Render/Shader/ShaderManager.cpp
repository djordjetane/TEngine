#include <precomp.h>
#include "ShaderManager.h"

namespace Render {
    bool ShaderManager::AddShader(const StringView& name)
    {
        m_Table[name] = std::make_unique<Shader>(directory + name.data());
        auto* shader  = m_Table[name].get();
        SMAASSERT(shader != nullptr, "Failed to add shader: {}", name);
		SMAASSERT(shader->Compile(), "Failed to compile shader: {}", name);
        LOG_TRACE("Compiled {}", name);
        return true;
    }

    Shader* ShaderManager::GetShader(const String& name) const { return m_Table.find(name)->second.get(); }

    bool ShaderManager::m_LoadDefaultShaders()
    {
        constexpr std::array<StringView,4> shaders{"sprite", "pbr", "screen", "skybox"};
        for (auto s : shaders)
        {
            SMAASSERT(AddShader(s), "Failed to add shader: {}", s);
        }
        return true;
    }
} // namespace Render
