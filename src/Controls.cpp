#include "../include/Controls.h"

#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

glm::mat4 _view_mat;
glm::mat4 _proj_mat;

glm::vec3 _pos = glm::vec3(0, 0, 5);

float _h_angle = 3.14f;
float _v_angle = 0.0f;

float _fov = 60.0f;

float _speed = 6.0f;
float _mouse_speed = 0.005f;

glm::mat4 Controls::get_view_mat()
{
    return _view_mat;
}

glm::mat4 Controls::get_proj_mat()
{
    return _proj_mat;
}

void Controls::compute_input_mat(GLFWwindow* window)
{
    static double last_time = glfwGetTime();
    
    glm::vec3 direction
    (
        glm::cos(_v_angle) * glm::sin(_h_angle),
        glm::sin(_v_angle),
        glm::cos(_v_angle) * glm::cos(_h_angle)
    );
    
    glm::vec3 right
    (
        glm::sin(_h_angle - 3.14f / 2.0f),
        0,
        glm::cos(_h_angle - 3.14f / 2.0f)
    );
    
    glm::vec3 up = glm::cross(right, direction);
    
    _proj_mat = glm::perspective(glm::radians(_fov), 4.0f / 3.0f, 0.1f, 100.0f);
    _view_mat = glm::lookAt(_pos, _pos + direction, up);
    
    double current_time = glfwGetTime();
    float delta_time = static_cast<float>(current_time - last_time);
    
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, 400, 300);
    
    _h_angle += _mouse_speed * static_cast<float>(400 - xpos);
    _v_angle += _mouse_speed * static_cast<float>(300 - ypos);
            
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        _pos += direction * delta_time * _speed;
    }
    
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        _pos -= direction * delta_time * _speed;
    }
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        _pos -= right * delta_time * _speed;
    }
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        _pos += right * delta_time * _speed;
    }
    
    last_time = current_time;
}