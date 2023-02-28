if(UNIX)  
  cmake_policy(SET CMP0072 NEW)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIR_ft2build}")
  
  set(GLAD_LIBRARY "${CMAKE_SOURCE_DIR}/3rdParty/Linux/OpenGL/libglad.a")
  set(IMGUI_LIBRARY "${CMAKE_SOURCE_DIR}/3rdParty/Linux/libimgui.a")      

else() # WIN32
  link_directories(${CMAKE_SOURCE_DIR}/3rdParty/Windows
                  ${CMAKE_SOURCE_DIR}/3rdParty/Windows/OpenGL           
                  )

  set(FREETYPE_INCLUDE_DIR_ft2build "${CMAKE_SOURCE_DIR}/3rdParty/freetype2/include")
endif(UNIX)

# Include paths for External libs
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/OpenGL/GLAD")
set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/imgui")
set(STB_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/stb_image")

# find the required packages
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/Config/OpenGL")
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")



# Grouping libs
if(WIN32)
  set(LIBS glad glfw3 opengl32 assimp freetype imgui)

else() # UNIX
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)  
  
  set(LIBS ${GLAD_LIBRARY} ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl 
      pthread freetype ${ASSIMP_LIBRARY} ${IMGUI_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
endif(WIN32)