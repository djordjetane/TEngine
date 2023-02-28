#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Engine {
    struct IMeshData
    {
        TVector<Vertex> Vertices;
        TVector<Uint> Indices;
    };

    struct ObjData
    {
        TVector<IMeshData> Meshes;
        TVector<Texture::ITexture> Textures;
    };

    class ObjImporter
    {
    public:
        ObjImporter(const String& path);
        ObjImporter(String parent, String name);

        void Import();
        ObjData& GetData();

    private:
        String m_Directory{};
        String m_Name{};
        ObjData m_Data{};

        void m_ProcessNode(aiNode* node, const aiScene* scene);
        void m_ProcessMesh(aiMesh* mesh, const aiScene* scene);
        void m_ProcessMaterialTextures(aiMaterial* mat, aiTextureType type);
    };
} // namespace Engine