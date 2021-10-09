cmake_minimum_required (VERSION 3.8)

project ("TEngine")


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
  message(STATUS "Defaulting configuration to Debug")
endif()

# Cmake options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/${CMAKE_PROJECT_NAME})

# Setting clang tidy
option(Clang-Tidy OFF)
if(Clang-Tidy)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

set(3RDPARTY "${CMAKE_SOURCE_DIR}/3rdParty")
include_directories(${3RDPARTY})

# System-based configs
if(UNIX)  
  # PASS   

else() # WIN32
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/Config/Windows")
  #include(Engine)
  include(Game)
endif(UNIX)


#include_directories(${ASSIMP_INCLUDE_DIR}
#                    ${FREETYPE_INCLUDE_DIR_ft2build}
#                    ${GLAD_INCLUDE_DIR}
#                    ${GLFW3_INCLUDE_DIR}
#                    ${IMGUI_INCLUDE_DIR}
#                    ${IMGUI_INCLUDE_DIR}/backends
#					${STB_IMAGE_INCLUDE_DIR}
#                    ${CMAKE_SOURCE_DIR}/External                    
#                    ${CMAKE_SOURCE_DIR}/Game/Source
#		            ${CMAKE_SOURCE_DIR}/Game/Source/Render
#                   )


				
