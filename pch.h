// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define BUFFER_OFFSET(x) ((void*)(x))

// add headers that you want to pre-compile here
#include <map>
#include <array>
#include <tuple>
#include <ctime>
#include <string>
#include <future>
#include <vector>
#include <random>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <stdexcept>
#include <filesystem>
#include <string_view>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#endif //PCH_H
