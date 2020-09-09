#pragma once

#include "System.h"

#include <noise/noise.h>
#include "../WorldGeneration/noiseutils.h"

using namespace noise;

class CloudModule : public module::Module {
    public:
        CloudModule();
        int GetSourceModuleCount() const;
        double GetValue(double x, double y, double z = 0.0) const;
};

class CloudSystem : public System {
    private:
        // clouds move along the Z-Axis
        int m_chunkOffset = 0;
        entt::entity m_cloudEntity;

        // perlin cloud generation
        module::Perlin m_perlinModule;
        CloudModule m_cloudModule;

        // recalculate cloud geometry and change mesh renderer buffer data
        void recalculateCloudGeometry();

        void _update(int dt) override;

    public:
        CloudSystem(Registry_T &registry);
};
