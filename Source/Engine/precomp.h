#pragma once

#ifdef WIN32
#include <Windows.h>
#include <winbase.h>
#endif

// extern libs
#include <glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define SPDLOG_NO_EXCEPTIONS
#include <spdlog/spdlog.h>
#include <stb_image.h>

#include <Core/Math.h>
#include <types.h>

#include <Engine.h>
