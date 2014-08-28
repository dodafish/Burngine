add_subdirectory(Shader)
add_subdirectory(Scene)

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)