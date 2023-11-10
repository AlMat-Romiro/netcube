#pragma once

#include <string>
#include <glm/glm.hpp>

struct Player
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    std::string nickname;
};
