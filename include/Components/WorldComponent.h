#include <glm/glm.hpp>
#include <unordered_map>
#include <entt/entt.hpp>

struct Block;

class HashFunction {
public:
	inline long operator()(const glm::ivec2& vec) const {
		return std::hash<float>()(vec.x) ^ (std::hash<float>()(vec.y) << 1);
	}
};

class WorldComponent {
private:
	std::unordered_map<glm::ivec2, entt::entity, HashFunction> chunksLookup = std::unordered_map<glm::ivec2, entt::entity, HashFunction>();;

public:
	WorldComponent() = default;

	unsigned int worldId = 0;

	static void getChunkCoords(glm::ivec3 position, glm::ivec2& chunk, glm::ivec3& chunkPosition);
	entt::entity getChunk(glm::ivec2 chunk) const;
	Block getBlock(entt::registry& registry, glm::ivec3 position) const;
	void setBlock(entt::registry& registry, glm::ivec3 position, Block block);

	void addChunk(entt::entity entity, glm::ivec2 position);
	void removeChunk(entt::entity entity);
};