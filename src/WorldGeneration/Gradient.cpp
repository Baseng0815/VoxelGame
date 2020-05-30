#include "../../include/WorldGeneration/Gradient.h"

using namespace noise::module;

Gradient::Gradient() : Module(GetSourceModuleCount()) {

}

int Gradient::GetSourceModuleCount() const { return 1; }

void Gradient::SetSourceModule(int index, const Module& module){
    if(index >= 0 && index < GetSourceModuleCount())
        m_pSourceModule[index] = &module;
}

const Module& Gradient::GetSourceModule(int index) const {
    if(index >= 0 && index < GetSourceModuleCount() - 1)
        return *m_pSourceModule[index];
}

void Gradient::SetStepSize(double stepSize) {
    m_stepSize = stepSize;    
}

double Gradient::GetStepSize() const {
    return m_stepSize;
}

double Gradient::GetValue(double x, double y, double z) const {
    double dx = (m_pSourceModule[0]->GetValue(x + m_stepSize, y, z) - m_pSourceModule[0]->GetValue(x - m_stepSize, y, z)) / (2 * m_stepSize);
    double dy = (m_pSourceModule[0]->GetValue(x, y + m_stepSize, z) - m_pSourceModule[0]->GetValue(x, y - m_stepSize, z)) / (2 * m_stepSize);
    double dz = (m_pSourceModule[0]->GetValue(x, y, z + m_stepSize) - m_pSourceModule[0]->GetValue(x, y, z - m_stepSize)) / (2 * m_stepSize);

    return sqrt(dx * dx + dy * dy + dz * dz);
}