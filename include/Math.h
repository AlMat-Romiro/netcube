#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

namespace Math
{
    // Взято из Game Programming Gems 4 1.10
    // Snatched from Game Programming Gems 4 1.10
    glm::vec3 smooth_damp(glm::vec3 from, glm::vec3 to, glm::vec3 &velocity, float time, float delta_time)
    {    
        float omega = 2.0f / time;
        float x = omega * delta_time;
        float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
        
        glm::vec3 change;
        change.x = from.x - to.x;
        change.y = from.y - to.y;
        change.z = from.z - to.z;
        
        glm::vec3 orig_to = to;
        
        float max_change = 6.0f * time;
        float sqrmag = change.x * change.x + change.y * change.y + change.z * change.z;
        if(sqrmag > (max_change * max_change))
        {
            float mag = glm::sqrt(sqrmag);
            change /= mag * max_change;
        }
        
        to = from - change;
        
        glm::vec3 temp = (velocity + omega * change) * delta_time;
        velocity = (velocity - omega * temp) * exp;
        
        glm::vec3 out = to + (change + temp) * exp;
        
        glm::vec3 orig_minus_from = orig_to - from;
        glm::vec3 out_minus_orig = out - orig_to;
        
        if(orig_minus_from.x * orig_minus_from.x + orig_minus_from.y * orig_minus_from.y + orig_minus_from.z * orig_minus_from.z > 0)
        {
            out = orig_to;
            velocity = (out - orig_to) / delta_time;
        }
        
        return out;
    }
}