# Variable for additional source files
set(SRC_FILES_ADD "")

# Append new source files to the list
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/WindowImpl.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/Event.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/Keyboard.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/VideoMode.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/GlContext.cpp)
LIST(APPEND SRC_FILES_ADD ${CMAKE_CURRENT_SOURCE_DIR}/GlEntity.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/WindowImpl.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Event.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Keyboard.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/VideoMode.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/GlContext.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/GlEntity.cpp)
endif()

if(${BURNGINE_OS_WINDOWS})
	add_subdirectory(Win32)
elseif(${BURNGINE_OS_UNIX})
	add_subdirectory(Unix)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} ${SRC_FILES_ADD} PARENT_SCOPE)