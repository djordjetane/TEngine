#pragma once
#include "Texture.h"

namespace Texture {
    class TextureManager
    {
    public:
        TextureManager() = default;
        ~TextureManager();
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager(TextureManager&&)                 = delete;
        /**
         * @brief Fetch loaded texture
         *
         * @param name Texture path
         *
         * @return Fetched texture or nullptr
         */
        ITexture* FindTexture(const String& name);

        /**
         * @brief Loads new texture from textures root directory
         *
         * @param path Relative or absolute path to resource
         * @param meta Metadata object
         *
         * @returns Created texture or nullptr
         */
        Texture2D* LoadTexture(String const& path, Metadata const& meta = {});
        Texture2D* LoadExternalTexture(String parentDir, const String& path, const Metadata& meta = {});
        Cubemap* LoadCubemap(const TArray<StringView, 6>& faces, const String& name = "cubemap");
        Cubemap* LoadCubemap(const StringView& facesDir, const String& name = "cubemap");

        bool ActivateTextures(TVector<String>& textures);

    private:
        std::unordered_map<String, TUniquePtr<ITexture>> m_Table;
        static constexpr char rootDir[] = "Resources/Textures/";

        ITexture* m_LoadTexture(String const& path, Metadata const& meta = {});
    };


} // namespace Texture
