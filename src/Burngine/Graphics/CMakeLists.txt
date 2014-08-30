add_subdirectory(Shader)
add_subdirectory(Scene)
add_subdirectory(Gui)

# Append new source files to the list
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Transformable3D.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Renderable.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/RenderTarget.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/VertexBuffer.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Transformable3D.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Renderable.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/RenderTarget.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/VertexBuffer.cpp)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)