#include "../include/Cuboid.h"

Cuboid::Cuboid(glm::vec3 position, float sizeX, float sizeY, float sizeZ) {
    min = max = position;    

    if(sizeX < 0.0f) min.x += sizeX;            
    else max.x += sizeX;        

    if(sizeY < 0.0f) min.y += sizeY;
    else max.y += sizeY;    

    if(sizeZ < 0.0f) min.z += sizeZ;    
    else max.z += sizeZ;    
}

std::unique_ptr<glm::vec3[]> Cuboid::getCornerPoints() const {
    glm::vec3 x = glm::vec3(max.x - min.x, 0, 0);
    glm::vec3 y = glm::vec3(0, max.y - min.y, 0);
    glm::vec3 z = glm::vec3(0, 0, max.z - min.z);
    
    return std::unique_ptr<glm::vec3[]>(new glm::vec3[] {
        min,
        min + x,
        min + y,
        min + z,
        min + x + y,
        min + x + z,
        min + y + z,
        min + x + y + z
    });  
}

bool Cuboid::intersects(const Cuboid& other) const {               
    if (max.x > other.min.x || max.y > other.min.y || max.z > other.min.z)
        return false;
    
    if (other.max.x > max.x || other.max.y > max.y || other.max.z > max.z)
        return false;

    return true;
}