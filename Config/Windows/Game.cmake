include(Engine)
set(NAME "Game")

set(Source_Root "${CMAKE_SOURCE_DIR}/Source/Game")
file(GLOB_RECURSE GameSrc
  "${Source_Root}/*.cpp"
  "${Source_Root}/*.hpp"
  "${Source_Root}/*.h"
  )
  

# Setting cmake executable
add_executable(${NAME} ${GameSrc})

# Grouping and forming source tree
foreach(SRC_file IN ITEMS ${GameSrc})
    get_filename_component(SRC_path "${SRC_file}" PATH)
    file(RELATIVE_PATH SRC_path_rel "${Source_Root}" "${SRC_path}")
    string(REPLACE "/" "\\" SRC_Group "${SRC_path_rel}")
    source_group("${SRC_Group}" FILES "${SRC_file}")
endforeach()


target_link_libraries(${NAME} PRIVATE ${LIBS})

if(MSVC)
    set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${Source_Root}")	    
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "${NAME}")
else()
    #target_precompile_headers(${NAME} INTERFACE ${Source_Root}/precomp.h)
    target_compile_definitions(${NAME} PUBLIC $<IF:$<CONFIG:Debug>,_DEBUG,NDEBUG>)
  endif(MSVC)

include_directories(${Source_Root})

