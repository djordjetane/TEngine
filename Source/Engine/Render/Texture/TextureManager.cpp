#include <precomp.h>

#include "Texture.h"
#include "TextureManager.h"


namespace Texture {

    /* Generates ID for texture target */
    static unsigned GenID(ETexTarget target = TEXTURE_2D);

    /* Stores raw image properties in RAII struct */
    static RawImage LoadRawImage(const String& name, int desiredChannels = 0);

    /* Converts number of channels to native enum format */
    static EPixelFormat GetFormat(int nChannels);

    /* Binds to array and specifies texture image to native lib */
    // static void BindTextureImage( ITexture& texture, Pixel* data);
    // static void BindTextureImage( ITexture& texture, RawImage& rawimage, EPixelFormat format = PF_NA);

    /*
    static void BindTextureImage( ITexture& texture, int width, int height, EPixelFormat inFormat,
                                  const Pixel* data, EPixelDataType type = PT_UBYTE );
                                  */
    /* Set texture parameter */
    template <typename T>
    static void SetTexParam(GLenum pname, T param, GLenum target = GL_TEXTURE_2D);
    /* Set texture parameters */
    template <typename T>
    static void SetTexParam(GLenum pname, const T* param, GLenum target = GL_TEXTURE_2D);


    /* Texture type parsers */
    // -------------------------------------
    /* str_ must be in format: texture_{type} */
    inline static TextureType TypeFromString(const String& str_)
    {
        return static_cast<TextureType>(std::toupper(str_[8]));
    }

    /* Returns formated string: texture_{type} */
    static String TypeToString(TextureType type);

    /* string in format: texture_{_type_}N
       where N is ordinary number of texture */
    inline static String TypeToStringN(TextureType type, unsigned N) { return TypeToString(type) + std::to_string(N); }


    Uint GenID(ETexTarget target)
    {
        Uint id;
        glGenTextures(1, &id);
        glBindTexture(target, id);

        return id;
    }

    RawImage LoadRawImage(const String& name, int desiredChannels)
    {
        int width, height, nChannels;

        Pixel* data = stbi_load(name.c_str(), &width, &height, &nChannels, desiredChannels);

        return {width, height, nChannels, data};
    }

    EPixelFormat GetFormat(int nChannels)
    {
        using ::EPixelFormat;
        switch(nChannels)
        {
        case 2: return PF_RG;
        case 3: return PF_RGB;
        case 4: return PF_RGBA;
        default: return PF_RED;
        }
    }

    /*
    void BindTextureImage( ITexture& texture, RawImage& rawimage, EPixelFormat inFormat )
    {
        auto [width, height, nChannels, data] = std::move( rawimage.Get() );

        BindTextureImage( texture, width, height, inFormat, data, PT_UBYTE);
    }
    */

    inline void apiGL_glTexImage2D(ITexture& texture, const Pixel* data)
    {
        auto& m = texture.Meta;
        glTexImage2D(m.Target, m.LOD, m.PFormat, m.Width, m.Height, 0, m.PFormat, m.PType, data);
    }

    void BindTextureImage(ITexture& texture, const Pixel* data)
    {
        glBindTexture(texture.Meta.Target, texture.ID);
        apiGL_glTexImage2D(texture, data);
        if(texture.Meta.Mipmap)
            glGenerateMipmap(texture.Meta.Target);
    }

    String TypeToString(TextureType type)
    {
        switch(type)
        {
        case TextureType::DIFFUSE: return String{"diffuse"};
        case TextureType::NORMAL: return String{"normal"};
        case TextureType::SPECULAR: return String{"specular"};
        case TextureType::HEIGHT: return String{"texture_height"};
        case TextureType::TEX_ERRORTYPE:
        default: break;
        }

        return String{"ErrorTextureType"};
    }


    ITexture LoadNativeTexture(const String& name, ETexTarget target, int desiredChannels)
    {
        auto rawIm{LoadRawImage(name, desiredChannels)};
        auto [width, height, nChannels, data] = rawIm.Get();

        nChannels = desiredChannels != 0 ? desiredChannels : nChannels;
        ITexture tex;
        tex.ID            = GenID(target);
        tex.Name          = name;
        tex.Meta.Width    = width;
        tex.Meta.Height   = height;
        tex.Meta.Channels = nChannels;
        tex.Meta.Target   = target;
        tex.Meta.PFormat  = GetFormat(nChannels);


        if(data == nullptr)
        {
            LOG_WARN("Failed to load texture\nTexture: {}", name);

#ifdef _DEBUG
            LOG_WARN("Error code: {}", glGetError());
#endif // _DEBUG

            tex.ID = 0;
            return tex;
        }

        // BindTextureImage( tex, rawIm, GetFormat(desiredChannels) );

        BindTextureImage(tex, data);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        LOG_INFO("Loaded texture {}", name);
        glBindTexture(target, 0);

        return tex;
    }


    // ---------------  Manager  --------------- //

    ITexture* TextureManager::m_LoadTexture(String const& path, Metadata const& meta)
    {
        if(path.empty())
            return nullptr;


        if(auto* tex = FindTexture(path))
            return tex;

        // m_Table[path] = nullptr;
        auto* texture = new Texture2D{LoadNativeTexture(path, meta.Target, meta.Channels)};
        if(texture->ID == 0)
            return nullptr;

        m_Table[path].reset(texture);
        return m_Table[path].get();
    }

    Texture2D* TextureManager::LoadTexture(const String& path, const Metadata& meta)
    {
        return static_cast<Texture2D*>(m_LoadTexture(rootDir + (path[0] != '/' ? path : path.substr(1)), meta));
    }

    Texture2D* TextureManager::LoadExternalTexture(String parentDir, const String& path, const Metadata& meta)
    {
        parentDir = (parentDir.back() == '/' ? parentDir : parentDir + '/');
        parentDir.append(path[0] == '/' ? path.substr(1) : path);

        return static_cast<Texture2D*>(m_LoadTexture(parentDir, meta));
    }

    Cubemap* TextureManager::LoadCubemap(const TArray<StringView, 6>& faces, const String& name)
    {
        if(m_Table.find(name) != m_Table.end())
        {
            static_cast<Cubemap*>(m_Table[name].get())->Delete();
            m_Table[name].reset();
        }

        auto* tex = new Cubemap;
        tex->LoadCubemap(faces);

        // m_Table[name] = TMakeUnique<ITexture>();
        m_Table[name].reset(tex);

        return static_cast<Cubemap*>(m_Table[name].get());
    }

    Cubemap* TextureManager::LoadCubemap(const StringView& facesDir, const String& name)
    {
        TArray<StringView, 6U> files; Uint nIt = 0;
        TVector<StringView> dirFiles{File::Manager::GetDirectoryContent(facesDir)};
        std::remove_if(dirFiles.begin(), dirFiles.end(), [](StringView& it) {return it.data() != nullptr;});
        std::copy_n(dirFiles.begin()+2, 6, files.begin());
        
        return LoadCubemap(files);
    }

    bool TextureManager::ActivateTextures(TVector<String>& textures)
    {
        for(size_t i = 0; i < textures.size(); i++)
            if(auto* tex = FindTexture(textures[i]))
                tex->Activate(i);

        return true;
    }

    TextureManager::~TextureManager()
    {
        for(auto& [k, v] : m_Table)
        {
            v->Delete();
            v.reset();
        }
    }

    ITexture* TextureManager::FindTexture(const String& name)
    {
        auto texIt = m_Table.find(name);
        if(texIt != m_Table.end())
            return texIt->second.get();
        return nullptr;
    }

} // namespace Texture
