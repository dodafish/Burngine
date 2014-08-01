add_subdirectory(System)
add_subdirectory(Window)

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)