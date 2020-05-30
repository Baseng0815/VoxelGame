#pragma once
#include <noise/noise.h>
#include <vector>
#include "Biome.h"

namespace noise::module {
    class TerrainModule : public Module {
    protected:
        struct BiomeNoise {
            Perlin perlin; 	
	        Perlin land;

	        Const desert;
	        Const flat;
	        Const ocean;
	        Const beach;

        	Select desertSelect;
	        Select oceanSelect;
	        Select biomeSelector;

            BiomeID getBiome(double x, double y, double z) const;
        } m_biomeNoise;               

        std::vector<Biome> m_terrains;

        Biome getBiome(BiomeID id) const;	
    public:
        TerrainModule();
        ~TerrainModule();

        int GetSourceModuleCount() const override;

        double GetValue(double x, double y, double z) const override;
        Biome getBiome(double x, double z) const;
    };
};