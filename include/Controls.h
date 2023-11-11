#pragma once

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

namespace Controls
{    
    void compute_input_mat(GLFWwindow* window);
    
    glm::mat4 get_view_mat();
    glm::mat4 get_proj_mat();

    glm::vec3 get_cam_pos();
}
