#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Shaders
{
    GLuint load(std::string vs_path, std::string fs_path);
}