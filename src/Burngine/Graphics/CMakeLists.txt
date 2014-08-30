add_subdirectory(Shader)
add_subdirectory(Scene)
add_subdirectory(Gui)

# Append new source files to the list
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Transformable.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Renderable.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/RenderTarget.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Transformable.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Renderable.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/RenderTarget.cpp)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)