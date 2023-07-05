# ---------------------------------------------------------------------------------
# Projects Settings
# ---------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()


if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COREVIDEO_LIBRARY CoreVideo)

    set(APPLE_LIBRARIES 
		${OPENGL_LIBRARY}
		${COCOA_LIBRARY}
		${IOKIT_LIBRARY}
		${COREVIDEO_LIBRARY}
	)
endif()

# ---------------------------------------------------------------------------------
# Cmake projects
# ---------------------------------------------------------------------------------
project(3DRenderScape)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(FBX_SUPPORT OFF)

set(LIBRARIES_SOURCE_PATH ${CMAKE_SOURCE_DIR}/libraries)
set(SRC_PATH ${CMAKE_SOURCE_DIR}/src)

include_directories(
	${LIBRARIES_SOURCE_PATH}/glad/include
	${LIBRARIES_SOURCE_PATH}/glfw/include
	${LIBRARIES_SOURCE_PATH}/imgui/include
    ${LIBRARIES_SOURCE_PATH}/imguiFileLoad
	${LIBRARIES_SOURCE_PATH}/glm/include
	${LIBRARIES_SOURCE_PATH}/stb/include
	${LIBRARIES_SOURCE_PATH}/assimp/include
	${LIBRARIES_SOURCE_PATH}/assimp
    ${LIBRARIES_SOURCE_PATH}/itugl/include
)

add_subdirectory(${CMAKE_SOURCE_DIR}/libraries)

set(libraries glad glfw assimp imgui itugl ${APPLE_LIBRARIES})
file(GLOB_RECURSE target_inc "src/*.h")
file(GLOB_RECURSE target_src "src/*.cpp")


file(GLOB_RECURSE shaders "src/shaders/*.vert" "src/shaders/*.frag" "src/shaders/*.geom" "src/shaders/*.glsl")

file(COPY ${SRC_PATH}/shaders
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

file(COPY ${SRC_PATH}/models
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

source_group("Shaders" FILES ${shaders})



set(TARGETNAME MyProgram)
add_executable(${TARGETNAME} ${target_inc} ${target_src} ${shaders})
target_link_libraries(${TARGETNAME} ${libraries})

