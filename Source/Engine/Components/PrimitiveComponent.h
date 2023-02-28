#pragma once

#include "Component.h"

#include <Render/Shader/Shader.h>
#include <Render/Texture/Texture.h>
#include "Vertex/VertexArray.h"

#include "data/sky.h"

namespace Component {
    struct IPrimitiveComponent : IComponent
    {
    };

    struct Material : IPrimitiveComponent
    {
        std::vector<Texture::Texture2D*> Textures;
        String ShaderName;
    };

    struct Sprite : IPrimitiveComponent
    {
        Texture::Texture2D* Texture;
        Vec2 Size{0.f};
        float Rotation = 0.f;
        Vec3 Color{1.f, 1.f, 1.f};
    };

    struct Mesh : IPrimitiveComponent
    {
        Render::VertexArray VArray;
    };

    struct Sky : Mesh
    {
        Sky() : Mesh()
        {
            auto* vBuffer = new Render::VertexBuffer(CubeMapVertices, 108);
            vBuffer->SetLayout({{Render::EShaderDataType::Float3, "aPos"}});
            VArray.AddVertexBuffer(vBuffer);
            VArray.SetElementBuffer(new Render::ElementBuffer(36));
        }

        Texture::Cubemap* Texture{};
        Render::Shader* Shader{};
    };
} // namespace Component