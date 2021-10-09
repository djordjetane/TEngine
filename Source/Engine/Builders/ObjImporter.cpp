#include <precomp.h>
#include "ObjImporter.h"


namespace Engine {

    ObjImporter::ObjImporter(const String& path)
        : m_Directory(path.substr(0, path.find_last_of('/'))), m_Name(path.substr(path.find_last_of('/')))
    {
    }

    ObjImporter::ObjImporter(String parent, String name) : m_Directory(std::move(parent)), m_Name(std::move(name)) {}

    void ObjImporter::Import()
    {
        Assimp::Importer importer;
        auto sceneFlags =
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

        const aiScene* scene = importer.ReadFile(m_Directory + '/' + m_Name, sceneFlags);
        // check for errors
        if(scene == nullptr || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || scene->mRootNode == nullptr)
        {
            LOG_ERROR("ERROR::ASSIMP::{}", importer.GetErrorString());
            return;
        }

        // process ASSIMP's root node recursively
        m_ProcessNode(scene->mRootNode, scene);
    }

    ObjData& ObjImporter::GetData() { return m_Data; }

    void ObjImporter::m_ProcessNode(aiNode* node, const aiScene* scene)
    {
        for(Uint i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            // m_Data.meshes.push_back(m_ProcessMesh(mesh, scene));
            m_ProcessMesh(mesh, scene);
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(Uint i = 0; i < node->mNumChildren; i++)
        {
            m_ProcessNode(node->mChildren[i], scene);
        }
    }

    void ObjImporter::m_ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        TVector<Vertex> vertices;
        TVector<Uint> indices;
        TVector<Texture::ITexture> textures;

        // walk through each of the mesh's vertices
        for(Uint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            Vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly
            // convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

            // positions
            vector.x        = mesh->mVertices[i].x;
            vector.y        = mesh->mVertices[i].y;
            vector.z        = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if(mesh->HasNormals())
            {
                vector.x      = mesh->mNormals[i].x;
                vector.y      = mesh->mNormals[i].y;
                vector.z      = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0] != nullptr) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x            = mesh->mTextureCoords[0][i].x;
                vec.y            = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x       = mesh->mTangents[i].x;
                vector.y       = mesh->mTangents[i].y;
                vector.z       = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x         = mesh->mBitangents[i].x;
                vector.y         = mesh->mBitangents[i].y;
                vector.z         = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding
        // vertex indices.
        for(Uint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(Uint j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        m_ProcessMaterialTextures(material, aiTextureType_DIFFUSE);
        m_ProcessMaterialTextures(material, aiTextureType_SPECULAR);
        m_ProcessMaterialTextures(material, aiTextureType_HEIGHT);
        m_ProcessMaterialTextures(material, aiTextureType_AMBIENT);

        // m_Data.emplace_back(IMeshData{vertices, indices}, textures);
        m_Data.Meshes.push_back({vertices, indices});
    }

    Texture::TextureType GetType(const aiTextureType& type)
    {
        switch(type)
        {
        case aiTextureType_DIFFUSE: return Texture::TextureType::DIFFUSE;
        case aiTextureType_SPECULAR: return Texture::TextureType::SPECULAR;
        case aiTextureType_HEIGHT: return Texture::TextureType::NORMAL;
        case aiTextureType_AMBIENT: return Texture::TextureType::HEIGHT;
        default: return Texture::TextureType::TEX_ERRORTYPE;
        }
    }

    void ObjImporter::m_ProcessMaterialTextures(aiMaterial* mat, aiTextureType type)
    {
        TVector<Texture::ITexture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(auto& tex : m_Data.Textures)
            {
                // auto textureName = textures_loaded[j].GetName().substr()
                if(std::strcmp(tex.Name.c_str(), str.C_Str()) == 0)
                {
                    textures.push_back(tex);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one.
                    // (optimization)
                    break;
                }
            }
            if(!skip)
            {
                // if texture hasn't been loaded already, load it
                Texture::Metadata meta;
                meta.Type = GetType(type);
                Texture::ITexture texture{0, m_Directory + "/" + str.C_Str(), meta};
                textures.push_back(texture);
                m_Data.Textures.push_back(texture); // store it as texture loaded for entire model, to ensure we won't
                // unnecesery load duplicate textures.
            }
        }
    }


} // namespace Engine
