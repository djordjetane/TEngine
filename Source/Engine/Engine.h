#pragma once
#include "precomp.h"

// Global data and utils
#include <Core/Math.h>
#include <Window/WindowData.h>
#include <types.h>
#include "Asserts.h"

// Classes
#include <Camera/Camera.h>
#include <Components/Components.h>
#include <Core/Logger/Log.h>
#include <Entities/Entity.h>
#include <Input/Keys.h>
#include <Render/Shader/Shader.h>
#include <Window/Window.h>

namespace Engine {
    using Camera = Render::Camera;
    using Entity = Entities::Entity;

    using EKeyCode       = Input::EKeyCode;
    using EKeyAction     = Input::EKeyAction;
    using EKeyEventState = Input::EKeyEventState;
} // namespace Engine

// Managers
#include <Entities/EntityManager.h>
#include <Render/Shader/ShaderManager.h>
#include <Render/Texture/TextureManager.h>

// Systems
#include <Input/InputSystem.h>
#include <Physics/PysicsSystem.h>
#include <Render/RenderSystem.h>

namespace Engine {
    using EntityManager  = Entities::EntityManager;
    using ShaderManager  = Render::ShaderManager;
    using TextureManager = Texture::TextureManager;
    using PhysicsManager = Physics::PhysicsSystem;
    using RenderSystem   = Render::RenderSystem;
    using InputSystem    = Input::InputSystem;
} // namespace Engine

#include <Core/File/Manager.h>
#include <Core/Application.h>