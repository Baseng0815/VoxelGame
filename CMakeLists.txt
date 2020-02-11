cmake_minimum_required(VERSION 3.12)

project(VoxelGame VERSION 1.0.0)

message("Using binary dir ${CMAKE_BINARY_DIR}")
message("Using project source dir ${VoxelGame_SOURCE_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lm -pthread -ggdb")

find_package(glfw3 3.3 REQUIRED)
if (GLFW_FOUND)
    message("GLFW was found")
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message("OpenGL was found")
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("GLEW was found")
endif()

find_package(Freetype REQUIRED)
if (FREETYPE_FOUND)
    message("Freetype was found")
endif()

message("GLFW LIBRARIES: ${GLFW_LIBRARIES}")
message("OpenGL LIBRARIES: ${OPENGL_LIBRARIES}")
message("GLEW LIBRARIES: ${GLEW_LIBRARIES}")

# ./libnoise
include_directories(/usr/include)

# freetype
include_directories(/usr/include/freetype2)

file(GLOB SOURCES
    include/*.h
    src/*.cpp)

add_executable(VoxelGame ${SOURCES})
target_link_libraries(VoxelGame
    ${OPENGL_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    libnoise.so
    libSOIL.so)