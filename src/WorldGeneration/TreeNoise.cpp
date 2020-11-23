#include "../../include/WorldGeneration/TreeNoise.hpp"

TreeNoise::TreeNoise() {
    m_noise.SetFrequency(0.7);
}

void TreeNoise::setRadius(int radius) {
    m_radius = radius;
}

bool TreeNoise::getValue(int x, int z) const {
    float value = m_noise.GetValue(x, 0, z);
    int radiusSquare = m_radius * m_radius;

    for (int dx = -m_radius; dx <= m_radius; dx++) {
        for (int dz = -m_radius; dz <= m_radius; dz++) {
            if (radiusSquare >= dx * dx + dz * dz) {
                if (m_noise.GetValue(x + dx, 0, z + dz) > value) {
                    return false;
                }
            }
        }
    }

    return true;
}