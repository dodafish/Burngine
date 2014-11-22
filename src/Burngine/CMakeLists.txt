# Variable for additional source files
set(SRC_FILES_ADD "")

add_subdirectory(System)
add_subdirectory(Window)
add_subdirectory(Graphics)

# Append new source files to the list
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/Cleaner.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Cleaner.cpp)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} ${SRC_FILES_ADD} PARENT_SCOPE)