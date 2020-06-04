#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <noise/noise.h>

class PerlinWorm {
private:
    void generateWorm(noise::module::Perlin& wormNoise, glm::vec3 head, int segmentsCount, float maxLength);

public:    
    int segmentsCount;    
    glm::vec3 head;
    std::vector<glm::vec3> segments;

    PerlinWorm() = default;
    PerlinWorm(noise::module::Perlin& wormNoise, glm::vec3 head, int segmentsCount, float maxLength);    
};

class WormPart : public PerlinWorm {
public:
    int index;
    glm::vec2 chunk;    
};

void DivideWorm(PerlinWorm worm, std::vector<WormPart>& parts);
