#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

class ObjectLoader
{
    public:
        ObjectLoader(std::string path);
        std::vector<glm::vec3> get_vertices();
        std::vector<glm::vec2> get_uv();
        std::vector<glm::vec3> get_normals();        
    
    private:
        std::string m_path;
        std::vector<glm::vec3> m_vertices;
        std::vector<glm::vec2> m_uv;
        std::vector<glm::vec3> m_normals;
};