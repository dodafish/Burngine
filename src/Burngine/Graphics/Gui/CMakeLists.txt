# Variable for additional source files
set(SRC_FILES_ADD "")

# Append new source files to the list
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/GuiNode.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Rectangle.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Transformable2D.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Sprite.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Font.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Label.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/GuiNode.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Rectangle.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Transformable2D.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Sprite.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Font.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Label.cpp)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} ${SRC_FILES_ADD} PARENT_SCOPE)