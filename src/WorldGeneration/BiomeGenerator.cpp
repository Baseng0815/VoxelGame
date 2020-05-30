#include "../../include/WorldGeneration/BiomeGenerator.h"
#include "../../include/Configuration.h"
#include "../../include/WorldGeneration/Biome.h"

BiomeGenerator::BiomeGenerator() {
	perlin.SetFrequency(0.001);
	perlin.SetOctaveCount(5);
	perlin.SetPersistence(0.25);

	land.SetFrequency(0.0025);
	land.SetOctaveCount(5);
	land.SetPersistence(0.2);
	land.SetLacunarity(2.1);

	desert.SetConstValue((int)BiomeID::BIOME_DESERT);
	flat.SetConstValue((int)BiomeID::BIOME_FLAT_TERRAIN);

	desertSelect.SetSourceModule(0, desert);
	desertSelect.SetSourceModule(1, flat);
	desertSelect.SetSourceModule(2, land);
	desertSelect.SetBounds(-1, 0);

	ocean.SetConstValue((int)BiomeID::BIOME_OCEAN);
	beach.SetConstValue((int)BiomeID::BIOME_BEACH);

	oceanSelect.SetSourceModule(0, beach);
	oceanSelect.SetSourceModule(1, ocean);
	oceanSelect.SetSourceModule(2, perlin);
	oceanSelect.SetBounds(0.1, 1);


	biomeSelector.SetSourceModule(0, oceanSelect);
	biomeSelector.SetSourceModule(1, desertSelect);
	biomeSelector.SetSourceModule(2, perlin);

	biomeSelector.SetBounds(-1, 0);	
}


void BiomeGenerator::generateBiomes(glm::ivec2 chunk, BiomeID** biomeMap) const {
	for (int cx = 0; cx < CHUNK_SIZE; cx++)
		for (int cz = 0; cz < CHUNK_SIZE; cz++) {
			BiomeID biome = (BiomeID)biomeSelector.GetValue(chunk.x * CHUNK_SIZE + cx, 0, chunk.y * CHUNK_SIZE + cz);

			biomeMap[cx][cz] = biome;
		}
}