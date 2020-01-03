cmake_minimum_required(VERSION 3.12)

message("Running src CMakeLists")
set(TARGET target_name)
file(GLOB SOURCES *.cpp)
add_executable(${TARGET} ${SOURCES})
