include_directories(include)
include_directories(#${TARGET_NAME} PRIVATE
        include
        ${EXTERNAL_LIBRARIES_SOURCE_PATH}/glad/include
        ${EXTERNAL_LIBRARIES_SOURCE_PATH}/glfw/include
        )
file(GLOB target_src "*.cpp" )
add_library(imguiFileLoad STATIC ${target_src})