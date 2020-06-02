#include "../include/CaveModule.h"
#include "../include/Utility.h"
#include "../include/Configuration.h"

CaveNoise::CaveNoise() : Module(GetSourceModuleCount()){
    m_seed = noise::module::DEFAULT_CAVENOISE_SEED;
}

void CaveNoise::SetSeed(int seed) {
    m_seed = seed;
}

int CaveNoise::GetSeed() const { return m_seed; }

int CaveNoise::GetSourceModuleCount() const { 
    return 0; 
}

double CaveNoise::GetValue(double x, double y, double z) const {
    glm::vec2 chunk = glm::vec2(x / CHUNK_SIZE, z / CHUNK_SIZE);

    int pointsCount;
    glm::vec3* points = GetBezierPoints(GetCavePoints(chunk, 5, m_seed), 5, &pointsCount);

    
}