#include <precomp.h>
#include "ShaderManager.h"

namespace Render {
    bool ShaderManager::AddShader(String& name)
    {
        m_Table[name] = std::make_unique<Shader>(directory + name);
        auto* shader  = m_Table[name].get();
        SMAASSERT(shader != nullptr, "Failed to add shader: {}", name);
        SMAASSERT(shader->Compile(), "Failed to compile shader: {}", name);

        LOG_TRACE("Compiled {}", name);
        return true;
    }

    Shader* ShaderManager::GetShader(const String& name) { return m_Table.find(name)->second.get(); }

} // namespace Render
