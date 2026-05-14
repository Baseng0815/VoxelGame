cmake_minimum_required(VERSION 3.16)
project(voxelgame CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(voxelgame ${SOURCES})

target_include_directories(voxelgame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# The code uses glm/gtx/* (hash, quaternion, transform), which are experimental.
target_compile_definitions(voxelgame PRIVATE GLM_ENABLE_EXPERIMENTAL)

# --- Dependencies ---
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)
find_package(EnTT REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)

# libnoise ships no CMake/pkg-config files, so locate it manually.
find_path(LIBNOISE_INCLUDE_DIR noise/noise.h)
find_library(LIBNOISE_LIBRARY NAMES noise libnoise)
if(NOT LIBNOISE_INCLUDE_DIR OR NOT LIBNOISE_LIBRARY)
    message(FATAL_ERROR "libnoise not found (looked for noise/noise.h and libnoise)")
endif()
target_include_directories(voxelgame PRIVATE ${LIBNOISE_INCLUDE_DIR})

target_link_libraries(voxelgame PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    Freetype::Freetype
    assimp::assimp
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json
    Threads::Threads
    ${LIBNOISE_LIBRARY}
)

# The game expects the Resources folder next to the binary.
add_custom_command(TARGET voxelgame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:voxelgame>/Resources
)

install(TARGETS voxelgame RUNTIME DESTINATION bin)
install(DIRECTORY Resources DESTINATION bin)
