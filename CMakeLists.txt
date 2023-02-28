cmake_minimum_required (VERSION 3.8)

project ("TEngine")


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
  message(STATUS "Defaulting configuration to Debug")
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin/${CMAKE_PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/Config")

option(API "OPENGL")

if(WIN32)
  if(${API} STREQUAL "OPENGL")
    message(STATUS "Configuring for OpenGL")
    include(OpenGL)
  elseif(${API} STREQUAL "DX12")
    message(STATUS "Configuring for DirectX 12")
    # dx12
  else()
    message(FATAL_ERROR "Unsupported API")  
  endif()
elseif(UNIX)
  # unix
else()
  message(FATAL_ERROR "Unsupported OS")
endif(WIN32)

set(Source_Root "${CMAKE_SOURCE_DIR}/Source")
set(Game_Source_Root "${CMAKE_SOURCE_DIR}/Source/Game")
file(GLOB_RECURSE GameSrc	
  "${Game_Source_Root}/*.cpp"
  "${Game_Source_Root}/*.hpp"
  "${Game_Source_Root}/*.h"
  )

set(Engine_Source_Root "${CMAKE_SOURCE_DIR}/Source/Engine")
file(GLOB_RECURSE EngineSrc
  "${Engine_Source_Root}/*.cpp"
  "${Engine_Source_Root}/*.hpp"
  "${Engine_Source_Root}/*.h"
  )

file(GLOB_RECURSE Shaders
  "${Source_Root}/Resources/*.vert"
  "${Source_Root}/Resources/*.frag"
  )

# Game files
set(ENAME "Engine")
set(NAME "Game")

file(
    GLOB_RECURSE Source_List 
    LIST_DIRECTORIES false
    ${EngineSrc}
	${Shaders}
)

file(
    GLOB_RECURSE GAMESource_List 
    LIST_DIRECTORIES false
    ${GameSrc}	
)

# Setting clang tidy
option(Clang-Tidy OFF)
if(Clang-Tidy)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

add_library(${ENAME} STATIC ${Source_List})
# Grouping and forming source tree
foreach(SRC_file IN ITEMS ${Source_List})
    get_filename_component(SRC_path "${SRC_file}" PATH)
    file(RELATIVE_PATH SRC_path_rel "${Source_Root}" "${SRC_path}")
    string(REPLACE "/" "\\" SRC_Group "${SRC_path_rel}")
    source_group("${SRC_Group}" FILES "${SRC_file}")
endforeach()

# Setting cmake executable
add_executable(${NAME} ${GAMESource_List})
# Grouping and forming source tree
foreach(SRC_file IN ITEMS ${GAMESource_List})
    get_filename_component(SRC_path "${SRC_file}" PATH)
    file(RELATIVE_PATH SRC_path_rel "${Game_Source_Root}" "${SRC_path}")
    string(REPLACE "/" "\\" SRC_Group "${SRC_path_rel}")
    source_group("${SRC_Group}" FILES "${SRC_file}")
endforeach()

target_link_libraries(${NAME} PUBLIC ${ENAME})
target_link_libraries(${NAME} PRIVATE ${LIBS})


if(MSVC)
                                                          # PCH Use   # inline exp
    # set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "/Yuprecomp.h /Ob1")         
    # set_source_files_properties("${Source_Root}/Game/precomp.cpp" PROPERTIES COMPILE_FLAGS "/Ycprecomp.h") # PCH create    
    set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "/Ob1")
	
	# PCH Use   # inline exp
    set_target_properties(${ENAME} PROPERTIES COMPILE_FLAGS "/Yuprecomp.h /Ob1")         
    set_source_files_properties("${Source_Root}/Engine/precomp.cpp" PROPERTIES COMPILE_FLAGS "/Ycprecomp.h") # PCH create
	
	
    set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${Source_Root}")	    
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "${NAME}")	        
	  
	       
    # CLANG-TIDY as default code analysis tool in VS
    if(Clang-Tidy)
      set_target_properties(${NAME} PROPERTIES VS_GLOBAL_RunCodeAnalysis "true")
      set_target_properties(${NAME} PROPERTIES VS_GLOBAL_EnableClangTidyCodeAnalysis "true")
      set_target_properties(${NAME} PROPERTIES VS_GLOBAL_ClangTidyChecks "clang-tidy")
      set_target_properties(${NAME} PROPERTIES VS_GLOBAL_EnableMicrosoftCodeAnalysis "false")
    endif()

else()
	target_precompile_headers(${ENAME} INTERFACE ${Source_Root}/Engine/precomp.h)
  target_compile_definitions(${ENAME} PUBLIC $<IF:$<CONFIG:Debug>,_DEBUG,NDEBUG>)
  # target_precompile_headers(${NAME} INTERFACE ${Source_Root}/Game/precomp.h)
  target_compile_definitions(${NAME} PUBLIC $<IF:$<CONFIG:Debug>,_DEBUG,NDEBUG>)  
endif(MSVC)


if(WIN32)									
  add_custom_command(TARGET ${NAME} PRE_LINK COMMAND 
              ${CMAKE_COMMAND} -E copy
              "${CMAKE_SOURCE_DIR}/3rdParty/Windows/OpenGL/assimp-vc140-mt.dll" $<TARGET_FILE_DIR:${NAME}>)


# Making symbolic link on unix for Resources
else()
  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND 
        ${CMAKE_COMMAND} -E create_symlink "${Source_Root}/Resources" 
        $<TARGET_FILE_DIR:${NAME}>/Resources)        

endif(WIN32)


# Suppressing warnings from external libs
if(CMAKE_COMPILER_IS_GNUCXX)  
    file(GLOB_RECURSE externHeaders "${CMAKE_SOURCE_DIR}/3rdParty/*.h" "${CMAKE_SOURCE_DIR}/3rdParty/*.hpp" "${Source_Root}/Engine/precomp.cpp")
    set_source_files_properties(${externHeaders} PROPERTIES COMPILE_FLAGS 
    "-Wno-shift-negative-value -Wno-implicit-fallthrough -Wno-unused-but-set-variable -Wno-sign-compare")        
endif()

include_directories(${ASSIMP_INCLUDE_DIR}
                    ${FREETYPE_INCLUDE_DIR_ft2build}
                    ${GLAD_INCLUDE_DIR}
                    ${GLFW3_INCLUDE_DIR}
                    ${GLM_INCLUDE_DIR}
                    ${IMGUI_INCLUDE_DIR}
                    ${IMGUI_INCLUDE_DIR}/backends
					          ${STB_IMAGE_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}/3rdParty                    
                    ${CMAKE_SOURCE_DIR}/Source/Engine					
		                ${CMAKE_SOURCE_DIR}/Source/Engine/Render
					          ${CMAKE_SOURCE_DIR}/Source/Game
                   )


				
