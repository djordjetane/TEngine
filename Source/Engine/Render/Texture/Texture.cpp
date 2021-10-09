#include <precomp.h>

#include "Texture.h"


namespace Texture {

    RawImage LoadRawImage(const String& name, int desiredChannels)
    {
        int width, height, nChannels;

        unsigned char* data = stbi_load(name.c_str(), &width, &height, &nChannels, desiredChannels);

        return {width, height, nChannels, data};
    }


    unsigned GenID(GLenum target)
    {
        unsigned id;
        glGenTextures(1, &id);
        glBindTexture(target, id);

        return id;
    }

    GLint GetFormat(int nChannels)
    {
        switch(nChannels)
        {
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return GL_RED;
        }
    }

    void BindTextureImage(unsigned id,
                          int width,
                          int height,
                          GLint internalformat,
                          const stbi_uc* data,
                          GLenum target,
                          bool genmm,
                          GLenum format,
                          GLint level,
                          GLenum type)
    {
        format = format != 0 ? format : internalformat;
        glBindTexture(target, id);
        glTexImage2D(target, level, internalformat, width, height, 0, format, type, data);
        if(genmm)
            glGenerateMipmap(target);
    }

    String TypeToString(TextureType type)
    {
        switch(type)
        {
        case TextureType::DIFFUSE:
            return String{"diffuse"};
            // return String{"texture_diffuse"};
        case TextureType::NORMAL:
            return String{"normal"};
            // return String{"texture_normal"};
        case TextureType::SPECULAR:
            return String{"specular"};
            // return String{"texture_specular"};
        case TextureType::HEIGHT:
            return String{"texture_height"};
            // return String{"texture_height"};
        case TextureType::TEX_ERRORTYPE:
        default: break;
        }

        return String{"ErrorTextureType"};
    }


    template <typename T>
    void SetTexParam(GLenum pname, T param, GLenum target)
    {
        glTexParameteri(target, pname, param);
    }

    template <typename T>
    void SetTexParam(GLenum pname, const T* param, GLenum target)
    {
        glTexParameteriv(target, pname, param);
    }

    void ITexture::Activate(Uint ord) const
    {
        glActiveTexture(GL_TEXTURE0 + ord);
        glBindTexture(Meta.Target, ID);
    }

    void ITexture::Deactivate(Uint ord) const
    {
        if(ID == 0)
            return;
        glActiveTexture(GL_TEXTURE0 + ord);
        glBindTexture(Meta.Target, 0);
    }

    void ITexture::Delete() const { glDeleteTextures(1, &ID); }

    void Texture2D::Activate(Uint ord) const
    {
        glActiveTexture(GL_TEXTURE0 + ord);
        glBindTexture(TEXTURE_2D, ID);
    }

    void Texture2D::Deactivate(Uint ord) const
    {
        if(ID == 0)
            return;
        glActiveTexture(GL_TEXTURE0 + ord);
        glBindTexture(TEXTURE_2D, 0);
    }

    bool Cubemap::LoadCubemap(const TArray<StringView, 6>& facesPaths)
    {
        unsigned id          = GenID(GL_TEXTURE_CUBE_MAP);
        unsigned textureXPos = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

        for(auto& face : facesPaths)
        {
            auto [width, height, nChannels, data]{LoadRawImage(face.data(), 3)};
            if(data == nullptr)
            {
                std::cout << "Cubemap m_textureID failed to load at path: " << face << std::endl;
                return false;
            }

            glTexImage2D(textureXPos++, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        ID            = id;
        Meta.Channels = 3;
        Meta.Type     = TextureType::DIFFUSE;

        LOG_INFO("Loaded cubemap");
        return true;
    }

    void Cubemap::Activate() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TEXTURE_CUBE_MAP, ID);
    }

    void Cubemap::Deactivate() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TEXTURE_CUBE_MAP, 0);
    }

} // namespace Texture
