#pragma once

#include "BlockData.h"
#include <map>
#include <string>

struct Block {
private:
    const static std::map<BlockType, std::string> blockNames;
public:

    Block(BlockType type = BlockType::BLOCK_AIR);

    BlockType type;

    std::string toString() const;
};
