#pragma once
#include <noise/noise.h>

using namespace noise::module;

namespace noise::module {

const int DEFAULT_CAVENOISE_SEED = 0;

class CaveNoise : public Module {
private:
    int m_seed;

public:
    CaveNoise();    

    void SetSeed(int seed);
    int GetSeed() const;

    int GetSourceModuleCount() const override;
    double GetValue(double x, double y, double z) const override;
};

}