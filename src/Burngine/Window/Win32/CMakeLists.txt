# Variable for additional source files
set(SRC_FILES_ADD "")

# Append new source files to the list
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/WindowImplWin32.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/WglContext.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/KeyboardImplWin32.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/WindowImplWin32.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/WglContext.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/KeyboardImplWin32.cpp)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} ${SRC_FILES_ADD} PARENT_SCOPE)