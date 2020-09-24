#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <functional>

namespace Math {
struct Plane;

struct Ray {
  private:
    glm::vec3 direction;

  public:
    glm::vec3 origin;
    Ray();

    Ray(glm::vec3 origin, glm::vec3 direction);

    glm::vec3 getPoint(float t) const;
    glm::vec3 getDirection() const;
    void setDirection(const glm::vec3 dir);

    bool intersects(const Plane &plane, float *t) const;

    glm::vec3 getFirstBlock(float maxT, std::function<bool(glm::vec3)> predicate) const;    
};
} // namespace Math