# VoxelGame

![Terrain](https://github.com/Baseng0815/VoxelGame/blob/master/docs/background.jpg)

## About the project
A voxel engine similar to minecraft written from scratch using C++ and OpenGL for educational purposes.
Current features include a GUI system, an ECS-driven engine using [entt](https://github.com/skypjack/entt),
and basic terrain generation using [noise](http://libnoise.sourceforge.net/). This has become quite a large project
over time which we didn't anticipate, so the whole code base is a mess and in dire need of a rework.

## Getting Started
This project was set up using CMake, so you need CMake to create the Makefile and a C++ compiler like gcc or clang to build the project.
First clone the repository and then setup and build like this:
```sh
    git clone https://github.com/Baseng0815/VoxelGame
    cd VoxelGame
    mkdir build
    cd build
    cmake ..
    cmake --build . (or make)
```

Requirements can be found in the CMakeLists.txt, but will also be listed here.
* C++20
* OpenGL
* [GLFW](https://www.glfw.org/)
* [GLEW](http://glew.sourceforge.net/)
* [Freetype](https://www.freetype.org/)
* [Assimp](https://github.com/assimp/assimp)
* Threads
* [entt](https://github.com/skypjack/entt)
* [nlohmann/json](https://github.com/nlohmann/json)
* [libnoise](http://libnoise.sourceforge.net/)
* SOIL

This project is being developed in collaboration with [PhiGei2000](https://github.com/PhiGei2000) who uses Windows, so the CMakeLists.txt
is pretty messed up and might not work for you. Properly setting up all the libraries and include directories is complicated and if you are
a CMake expert, don't hesitate to send corrections or improvements.

## Usage
Just run the binary. You need to make sure that the Resources folder is in the same directory as the binary, so it might be
beneficial to symlink it or to move the binary.

## Roadmap
* Cleaning up the codebase, i.e. modularize and reduce dependencies, making clear design choices etc.
* Use rendering techniques like occlusion culling and bump maps for better graphics
* Frustum culling to improve performance
* Player models and a third person camera
* Other entities with behavior
* World generation (structures etc.)
* Plant growth
* Proper item drops and an item system
* Reduce dynamic generation of geometry to a minimum
* Implement water flow
* ...

## Contact
E-Mail: [bastian.engel00@gmail.com](mailto:bastian.engel00@gmail.com)
