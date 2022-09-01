#pragma once

/* STL */
#include <array>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <filesystem>

typedef unsigned Uint;
typedef uint8_t Uint8;
typedef unsigned char Uchar;
typedef Uchar Pixel;
template <typename T, Uint Size>
using TArray = std::array<T, Size>;
template <typename T>
using TVector = std::vector<T>;
typedef TVector<float> FVector;
using Image = TVector<FVector>;
template <typename T>
using TQueue     = std::queue<T>;
using String     = std::string;
using StringView = std::string_view;
template <typename T>
using TUniquePtr = std::unique_ptr<T>;
template <typename T>
constexpr auto TMakeUnique = std::make_unique<T>;
template <typename T>
using TSharedPtr = std::shared_ptr<T>;
template <typename K, typename V>
using TMap = std::map<K, V>;
template <typename K, typename V>
using THashMap = std::unordered_map<K, V>;
template <typename... Ts>
using TTuple = std::tuple<Ts...>;
using Mutex  = std::mutex;

// ------------------------------------------------

/* 3rd */
#include <spdlog/spdlog.h>
using LoggerAPI = spdlog::logger;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

template <int L, typename T>
using Vec  = glm::vec<L, T>;
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat2 = glm::mat2;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;
using Quat = glm::quat;

template<typename T>           
constexpr auto MakeVec1 = glm::make_vec1<T>;
template<typename T>
constexpr auto MakeVec2 = glm::make_vec2<T>;
template<typename T>
constexpr auto MakeVec3 = glm::make_vec3<T>;
template<typename T>           
constexpr auto MakeVec4 = glm::make_vec4<T>;
template<typename T>
constexpr auto MakeQuat = glm::make_quat<T>;

// ================================================

/*** APIs ***/

/** GL **/
#ifdef GLAPI
typedef GLenum ETexTarget;
/** @defgroup Texture target types
 *
 * @brief Types of targets bounding texture
 */
#define TEXTURE_1D GL_TEXTURE_1D
#define TEXTURE_2D GL_TEXTURE_2D
#define TEXTURE_3D GL_TEXTURE_3D
#define TEXTURE_1D_ARRAY GL_TEXTURE_1D_ARRAY
#define TEXTURE_2D_ARRAY GL_TEXTURE_2D_ARRAY
#define TEXTURE_RECTANGLE GL_TEXTURE_RECTANGLE
#define TEXTURE_CUBE_MAP GL_TEXTURE_CUBE_MAP
#define TEXTURE_BUFFER GL_TEXTURE_BUFFER
#define TEXTURE_2D_MULTISAMPLE GL_TEXTURE_2D_MULTISAMPLE
#define TEXTURE_2D_MULTISAMPLE_ARRAY GL_TEXTURE_2D_MULTISAMPLE_ARRAY

/** @enum EPixelDataType
 *
 * @brief Data type of the pixel data
 */
enum EPixelDataType
{
    PT_UBYTE  = GL_UNSIGNED_BYTE,
    PT_BYTE   = GL_BYTE,
    PT_USHORT = GL_UNSIGNED_SHORT,
    PT_SHORT  = GL_SHORT,
    PT_UINT   = GL_UNSIGNED_INT,
    PT_INT    = GL_INT,
    PT_FLOAT  = GL_FLOAT
};

/** @enum EPixelFormat
 *
 * @brief Pixel format types
 */
enum EPixelFormat
{
    PF_NA    = 0,
    PF_RED   = GL_RED,
    PF_RG    = GL_RG,
    PF_RGB   = GL_RGB,
    PF_BGR   = GL_BGR,
    PF_RGBA  = GL_RGBA,
    PF_BGRA  = GL_BGRA,
    PF_SRGB  = GL_SRGB,
    PF_SRGBA = GL_SRGB_ALPHA
};

// GLFW
using NativeWindow = GLFWwindow;

// ------------------------------------------------
#endif

// ================================================

/* Abstractions */
struct VertexPNT
{
    Vec3 Position{0.f};
    Vec3 Normal{0.f};
    Vec2 TexCoords{0.f};
};

struct Vertex : VertexPNT
{
    Vec3 Tangent{0.f};
    Vec3 Bitangent{0.f};
};


template <typename K, typename V>
struct TPair
{
    K left;
    V right;
};
