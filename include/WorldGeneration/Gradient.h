#pragma once
#include <noise/noise.h>
#include "noiseutils.h"

namespace noise::module {
    class Gradient : public Module {
    protected:
        double m_stepSize = 0.01;        
    
    public:
        Gradient();

        int GetSourceModuleCount() const override;
        void SetSourceModule(int index, const Module& module) override;
        const Module& GetSourceModule(int index) const override; 
        
        void SetStepSize(double stepSize);
        double GetStepSize() const;        

        double GetValue(double x, double y, double z) const override;
    };
};