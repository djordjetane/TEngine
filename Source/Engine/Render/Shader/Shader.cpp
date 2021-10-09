#include <precomp.h>

#include <utility>
#include "Shader.h"

#define SHADEREXIST(s, path) SMAASSERT(s, "ERROR: Shader error (path= {} ):\nFile does not exist", path)

#define SHADERREAD(s, path) SMAASSERT(s, "ERROR: Failed to read vertex shader: (path= {}):\n", vShaderPath)

Render::Shader::Shader(String shaderName) : m_name(std::move(shaderName)) {}

bool Render::Shader::Compile()
{
    // String shaderPath{shadersDir + m_name};
    String shaderPath{m_name};

    auto vShaderPath{shaderPath + (".vert")};
    auto fShaderPath{shaderPath + (".frag")};

    std::ifstream vertexStream{vShaderPath};
    SHADEREXIST(vertexStream.is_open(), vShaderPath);

    std::ifstream fragStream{fShaderPath};
    SHADEREXIST(fragStream.is_open(), fShaderPath);

    std::stringstream vertexBuf;
    SHADERREAD(vertexBuf << vertexStream.rdbuf(), vShaderPath);
    std::stringstream fragBuf;
    SHADERREAD(fragBuf << fragStream.rdbuf(), fShaderPath);

    String vertexCode{vertexBuf.str()};
    String fragCode{fragBuf.str()};

    const char* vShaderCode = vertexCode.c_str();
    auto vertex             = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    SASSERT(m_CheckCompileErrors(vertex, "VERTEX"));

    const char* fShaderCode = fragCode.c_str();
    auto fragment           = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    SASSERT(m_CheckCompileErrors(fragment, "FRAGMENT"));

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    SASSERT(m_CheckCompileErrors(m_ID, "PROGRAM"));

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

void Render::Shader::ActivateShader() { glUseProgram(m_ID); }

void Render::Shader::DeactivateShader() { glUseProgram(0); }

void Render::Shader::SetValue(const String& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Render::Shader::SetValue(const String& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Render::Shader::SetValue(const String& name, unsigned value) const
{
    glUniform1ui(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Render::Shader::SetValue(const char* name, float val1, float val2, float val3) const
{
    glUniform3f(glGetUniformLocation(m_ID, name), val1, val2, val3);
}

void Render::Shader::SetVec2(const String& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Render::Shader::SetVec2(const String& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

// ------------------------------------------------------------------------
void Render::Shader::SetVec3(const String& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Render::Shader::SetVec3(const String& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

// ------------------------------------------------------------------------
void Render::Shader::SetVec4(const String& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Render::Shader::SetVec4(const String& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

// ------------------------------------------------------------------------
void Render::Shader::SetMat2(const String& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void Render::Shader::SetMat3(const String& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void Render::Shader::SetMat4(const String& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

char infoLog[1024]; // NOLINT
inline bool Render::Shader::m_CheckCompileErrors(Uint shader, const char* type) const
{
    int success;
    if(std::strcmp(type, "PROGRAM") != 0)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(success == 0)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {} \n{} \n-- "
                      "-------------------------------------------------- - -- \n",
                      type, infoLog);
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(success == 0)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}\n -- "
                      "--------------------------------------------------- -- ",
                      type, infoLog);
            return false;
        }
    }

    return true;
}
