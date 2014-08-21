# Variable for additional source files
set(SRC_FILES_ADD "")

# Append new source files to the list
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/NonCopyable.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/NonInstancable.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Time.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Clock.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/NonCopyable.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/NonInstancable.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Time.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Clock.cpp)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} ${SRC_FILES_ADD} PARENT_SCOPE)