set(NAME "Engine")

include_directories(${3RDPARTY}/Windows/DX12)
set(LIBS)

set(Source_Root "${CMAKE_SOURCE_DIR}/Source/Engine")
file(GLOB_RECURSE EngineSrc
  "${Source_Root}/*.cpp"
  "${Source_Root}/*.hpp"
  "${Source_Root}/*.h"
  )

add_library(${NAME} ${EngineSrc})

# Grouping and forming source tree
foreach(SRC_file IN ITEMS ${EngineSrc})
    get_filename_component(SRC_path "${SRC_file}" PATH)
    file(RELATIVE_PATH SRC_path_rel "${Source_Root}" "${SRC_path}")
    string(REPLACE "/" "\\" SRC_Group "${SRC_path_rel}")
    source_group("${SRC_Group}" FILES "${SRC_file}")
endforeach()

set(LIBS ${LIBS} ${NAME})

if(MSVC)
                                                          # PCH Use   # inline exp
    set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "/Yuprecomp.h /Ob1")         
    set_source_files_properties("${Source_Root}/precomp.cpp" PROPERTIES COMPILE_FLAGS "/Ycprecomp.h") # PCH create    
	
	
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
  target_precompile_headers(${NAME} INTERFACE ${Source_Root}/precomp.h)
  target_compile_definitions(${NAME} PUBLIC $<IF:$<CONFIG:Debug>,_DEBUG,NDEBUG>)
endif(MSVC)

include_directories(${Source_Root})

unset(Source_Root)
unset(NAME)