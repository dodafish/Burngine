# Variable for additional source files
set(SRC_FILES_ADD "")

# Append new source files to the list
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Transformable3D.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Model.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Instance.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Material.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/PointLight.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/DirectionalLight.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/SpotLight.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/AssetLoader.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Billboard.cpp)
LIST(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Skybox.cpp)

# Print added source files if required
if(PRINT_SOURCE_FILES)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Transformable3D.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Model.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Instance.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Material.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/PointLight.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/DirectionalLight.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/SpotLight.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/AssetLoader.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Billboard.cpp)
	message(STATUS "Added source file: " ${CMAKE_CURRENT_SOURCE_DIR}/Skybox.cpp)
endif()

# Pass source file list to parent directory
set(SRC_FILES ${SRC_FILES} ${SRC_FILES_ADD} PARENT_SCOPE)