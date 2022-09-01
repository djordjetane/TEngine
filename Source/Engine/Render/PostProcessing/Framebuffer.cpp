#include <precomp.h>

#include "Framebuffer.h"
#include "fbData.h"

namespace Render {
    bool Framebuffer::Init(Shader* _shader)
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Data.vertices), &Data.vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

        shader = _shader;

        shader->ActivateShader();
        shader->SetValue("screenTexture", 0);

        glGenFramebuffers(1, &nativeFb);
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFb);

        glGenTextures(1, &textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WND_WIDTH, WND_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

        glGenRenderbuffers(1, &m_RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WND_WIDTH, WND_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
        SMASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                 "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        shader->DeactivateShader();
        return true;
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteRenderbuffers(1, &m_RBO);
    }

    void Framebuffer::BindSceneBegin() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFb);
        glEnable(GL_DEPTH_TEST);
    }

    void Framebuffer::BindSceneEnd() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        shader->ActivateShader();
        shader->SetVec2("frameBufSize", WindowData::W, WindowData::H);
        shader->SetValue("AA", WindowData::fxaa);

        glBindVertexArray(m_VAO);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, Data.n_triangles);
        glBindTexture(GL_TEXTURE_2D, 0);
        shader->DeactivateShader();
    }
} // namespace Render
