#pragma once

#include <GL/glew.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <unordered_map>
#include <mutex>
#include <future>
#include <string>
#include <memory>
#include <iostream>

struct Block;

class HashFunction {
public:
    inline long operator()(const glm::vec2& vec) const {
        return std::hash<float>()(vec.x) ^ (std::hash<float>()(vec.y) << 1);
    }    
};

struct ChunkComponent {
private:
    static std::unordered_map<glm::vec2, entt::entity, HashFunction> chunksLookup;

public:
    Block*** blocks = nullptr;
    std::mutex *blockMutex;

    std::atomic_bool verticesOutdated   = false;
    std::atomic_bool threadActiveOnSelf = false;
    std::atomic_bool chunkBlocksCreated = false;

    int chunkX, chunkZ;

    ChunkComponent(int chunkX, int chunkZ);
    ChunkComponent(const ChunkComponent& other);
    ChunkComponent& operator=(const ChunkComponent& other);

    friend std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk);

    static void getChunkCoords(int x, int y, int z, int& chunkX, int& chunkZ, int& cx, int& cy, int& cz);
    static entt::entity getChunk(int chunkX, int chunkZ);
    static Block getBlock(entt::registry& registry, int x, int y, int z);
    static void setBlock(entt::registry& registry, int x, int y, int z, Block block);

    static void addChunk(entt::entity entity, int chunkX, int chunkZ);
    static void removeChunk(entt::entity entity);
};
