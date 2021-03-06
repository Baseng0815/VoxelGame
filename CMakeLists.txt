cmake_minimum_required(VERSION 3.12)

project(VoxelGame VERSION 1.0.0)

message("Using binary dir ${CMAKE_BINARY_DIR}")
message("Using project source dir ${VoxelGame_SOURCE_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_BUILD_TYPE Debug)


if(WIN32)
    message("WIN32")
    
    set(CMAKE_CXX_STANDARD 20)
    set(CAMKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GCTHREADS:16")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /NODEFAULTLIB:MSVCRT")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

    include_directories("C:/lib/assimp/include")    
    include_directories("C:/lib/entt-master/src")
    include_directories("C:/lib/GLEW/include")
    include_directories("C:/lib/GLFW/include")
    include_directories("C:/lib/GLM/include")
    include_directories("C:/lib/libnoise/include")
    include_directories("C:/lib/nlohmannjson/include")
    include_directories("C:/lib/freetype2/include")
    include_directories("C:/lib/freetype2/include/freetype2")
    include_directories("C:/lib/SOIL/src")

    find_library(ASSIMP_LIBRARIES assimp-vc142-mt C:/lib/assimp/bin/Win32)
    find_library(GLEW_LIBRARIES glew32 C:/lib/GLEW/lib/Release/Win32)
    find_library(GLFW_LIBRARIES glfw3 C:/lib/GLFW/lib)
    find_library(FREETYPE_LIBRARIES freetype C:/lib/freetype2/Win32)
    find_library(LIBNOISE_LIBRARIES libnoise C:/lib/libnoise/bin)
    find_library(SOIL_LIBRARIES SOIL C:/lib/SOIL/lib)

    find_package(OpenGL REQUIRED)
    if (OPENGL_FOUND)
        message("OpenGL was found")
    endif()

    file(GLOB_RECURSE SOURCES
        include/*.hpp
        src/*.cpp)

    add_executable(VoxelGame ${SOURCES})
    target_link_libraries(VoxelGame "-SAFESEH:NO"
        ${ASSIMP_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${LIBNOISE_LIBRARIES}
        ${SOIL_LIBRARIES})

else()
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")

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

    find_package(Assimp REQUIRED)
    if (Assimp_FOUND)
        message("assimp was found")
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    if (THREADS_FOUND)
        message("Threads was found")
    endif()

    message("GLFW LIBRARIES: ${GLFW_LIBRARIES}")
    message("OpenGL LIBRARIES: ${OPENGL_LIBRARIES}")
    message("GLEW LIBRARIES: ${GLEW_LIBRARIES}")
    message("Freetype LIBRARIES: ${FREETYPE_LIBRARIES}")
    message("Assimp LIBRARIES: ${ASSIMP_LIBRARIES}")

    file(GLOB_RECURSE SOURCES
        include/*.hpp
        src/*.cpp)

    add_executable(VoxelGame ${SOURCES})

    # add include directories
    target_include_directories(VoxelGame PUBLIC
        /usr/include/entt)

    # add libraries
    target_link_libraries(VoxelGame
        ${OPENGL_LIBRARIES}
        glfw ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        libnoise.so
        libSOIL.so
        Threads::Threads)
endif()
