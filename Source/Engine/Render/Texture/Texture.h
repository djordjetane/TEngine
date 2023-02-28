#pragma once

namespace Texture {
    /**
     * @enum TextureType
     *
     */
    enum class TextureType
    {
        DIFFUSE = 0,
        NORMAL,
        SPECULAR,
        HEIGHT,
        TEX_ERRORTYPE
    };

    /** @struct Metadata
     *
     * @brief Metadata wrapper
     *
     * @property channels Number of color channels, where default 0 means native
     * @property type One of four basic texture types
     * @property target Target type to witch texture is bound
     */
    struct Metadata
    {
        int Width            = 0;
        int Height           = 0;
        Uint Channels        = 0;
        TextureType Type     = TextureType::DIFFUSE;
        ETexTarget Target    = TEXTURE_2D;
        bool Mipmap          = true;
        Uint LOD             = 0;
        EPixelFormat PFormat = PF_NA;
        EPixelDataType PType = PT_UBYTE;
    };


    struct ITexture
    {
        Uint ID;
        String Name;
        Metadata Meta;
        void Activate(unsigned ord = 0) const;
        void Deactivate(unsigned ord = 0) const;
        void Delete() const;
    };

    struct Texture2D : ITexture
    {
        void Activate(unsigned ord = 0) const;
        void Deactivate(unsigned ord = 0) const;
    };


    struct Cubemap : ITexture
    {
        bool LoadCubemap(const TArray<StringView, 6>& facesPaths);
        void Activate() const;
        void Deactivate() const;
    };

    struct RawImage
    {
        int width, height, nChannels;
        Pixel* data = nullptr;

        using Properties = std::tuple<int, int, int, Pixel*>;
        Properties Get() { return {width, height, nChannels, data}; }

        Pixel* Set(const char* path, int desiredChannels = 0)
        {
            stbi_image_free(data);
            data = stbi_load(path, &width, &height, &nChannels, desiredChannels);
            return data;
        }

        ~RawImage() { stbi_image_free(data); }
    };

} // namespace Texture
