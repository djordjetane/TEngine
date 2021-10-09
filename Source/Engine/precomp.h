#pragma once

// #define _HAS_EXCEPTIONS 0
#define SPDLOG_NO_EXCEPTIONS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             
#endif

// MS headers
#include <Windows.h>
// #include <initguid.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
// #include <wrl/client.h>
#include "d3dx12.h"
#include <io.h>
#include <fcntl.h>

// STL
#include <memory>

#include "Engine.h"