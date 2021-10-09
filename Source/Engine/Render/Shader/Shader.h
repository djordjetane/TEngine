#pragma once

namespace Render {
    class Renderer;
    class Mesh;

    class Shader
    {
    public:
        Shader(String shaderName);

        bool Compile();

        void ActivateShader();

        void DeactivateShader();

        void SetValue(const String& name, int value) const;

        void SetValue(const String& name, float value) const;

        void SetValue(const String& name, unsigned value) const;

        void SetValue(const char* name, float val1, float val2, float val3) const;

        void SetVec2(const String& name, const glm::vec2& value) const;

        void SetVec2(const String& name, float x, float y) const;

        void SetVec3(const String& name, const glm::vec3& value) const;

        void SetVec3(const String& name, float x, float y, float z) const;

        void SetVec4(const String& name, const glm::vec4& value) const;

        void SetVec4(const String& name, float x, float y, float z, float w);

        void SetMat2(const String& name, const glm::mat2& mat) const;

        void SetMat3(const String& name, const glm::mat3& mat) const;

        void SetMat4(const String& name, const glm::mat4& mat) const;

        // inline static const char* shadersDir = "Resources/Shaders/";

    private:
        unsigned m_ID = 0;
        String m_name;

        bool m_CheckCompileErrors(Uint shader, const char* type) const;

        friend Renderer;
        friend Mesh;
    };
} // namespace Render
