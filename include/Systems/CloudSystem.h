#pragma once

#include "System.h"

#include <noise/noise.h>

#include "../WorldGeneration/noiseutils.h"

#include "../Resources/Geometry.h"

using namespace noise;

struct EnterChunkEvent;

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

        // player chunk position
        int m_chunkPosX, m_chunkPosZ;
        entt::entity m_cloudEntity;

        CallbackHandle<const EnterChunkEvent&> m_enterChunkHandle;
        void handleEnterChunk(const EnterChunkEvent&);

        // perlin cloud generation
        module::Perlin m_perlinModule;
        CloudModule m_cloudModule;
        Geometry m_geometry;

        // recalculate cloud geometry and change mesh renderer buffer data
        void recalculateCloudGeometry();

        void _update(int dt) override;

    public:
        CloudSystem(Registry_T &registry);
};
