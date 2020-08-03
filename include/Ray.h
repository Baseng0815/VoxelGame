#pragma once

#include <glm/glm.hpp>

struct Ray { 
private:
    glm::vec3 direction;

public:
    glm::vec3 origin;
    Ray();

    Ray(glm::vec3 origin, glm::vec3 direction);

    glm::vec3 getPoint(float lambda) const;
    glm::vec3 getDirection() const;
    void setDirection(const glm::vec3 dir);    
};