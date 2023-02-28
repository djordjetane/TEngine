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
