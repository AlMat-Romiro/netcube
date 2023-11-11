#include "../include/Controls.h"
#include "../include/Math.h"

#include <vector>

#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 _view_mat;
glm::mat4 _proj_mat;

glm::vec3 _pos = glm::vec3(0, 0, 5);
glm::vec3 _smooth_pos;

float _h_angle = 3.14f;
float _v_angle = 0.0f;

float _fov = 60.0f;

float _max_speed = 6.0f;
glm::vec2 _speed(0.0f);
float _acceleration = 0.5f;
float _mouse_speed = 0.005f;

std::vector<int> _last_scan = {0, 0};

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
        _last_scan.at(0) = GLFW_KEY_W;
        
        if(_speed.x < _max_speed)
        {
            _speed.x += _acceleration;
        }
        
        _pos += direction * delta_time * _speed.x;
    }
    
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        _last_scan.at(0) = GLFW_KEY_S;
    
        if(_speed.x < _max_speed)
        {
            _speed.x += _acceleration;
        }
        
        _pos -= direction * delta_time * _speed.x;
    }
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        _last_scan.at(1) = GLFW_KEY_A;
    
        if(_speed.y < _max_speed)
        {
            _speed.y += _acceleration;
        }
        
        _pos -= right * delta_time * _speed.y;
    }
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        _last_scan.at(1) = GLFW_KEY_D;
    
        if(_speed.y < _max_speed)
        {
            _speed.y += _acceleration;
        }
        
        _pos += right * delta_time * _speed.y;
    }
    
    if((_speed.x > 0.0f || _speed.y > 0.0f) && (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS
    || glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS))
    {
        _speed -= _max_speed * delta_time;
        if(_speed.x <= 0.1f)
        {
            _speed.x = 0.0f;
            _last_scan.at(0) = 0;
        }
        else if(_speed.y <= 0.1f)
        {
            _speed.y = 0.0f;
            _last_scan.at(1) = 0;
        }
        
        switch(_last_scan.at(0))
        {
            case GLFW_KEY_W:
                _pos += direction * delta_time * _speed.x;
                break;
            
            case GLFW_KEY_S:
                _pos -= direction * delta_time * _speed.x;
                break;
        }
            
        switch(_last_scan.at(1))
        {
            case GLFW_KEY_A:
                _pos -= right * delta_time * _speed.y;
                break;
    
            case GLFW_KEY_D:
                _pos += right * delta_time * _speed.y;
                break;
        }
    } 
    
    if(_last_scan.size() > 2)
    {
        _last_scan.resize(2);
    }
        
    last_time = current_time;
}