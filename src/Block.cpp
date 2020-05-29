#include "../include/Block.h"

const std::map<BlockType, std::string> Block::blockNames = {
	{ BlockType::BLOCK_AIR, "air" },
	{ BlockType::BLOCK_BOOLSHELF, "boolshelf" },
	{ BlockType::BLOCK_BRICKS, "bricks" },
	{ BlockType::BLOCK_DIAMOND, "diamond" },
	{ BlockType::BLOCK_DIRT, "dirt" },
	{ BlockType::BLOCK_EMERALD, "emerald" },
	{ BlockType::BLOCK_GOLD, "gold" },
	{ BlockType::BLOCK_GRASS, "grass" },
	{ BlockType::BLOCK_GRAVEL, "gravel" },
	{ BlockType::BLOCK_IRON, "iron" },
	{ BlockType::BLOCK_ORE_COAL, "coal ore" },
	{ BlockType::BLOCK_ORE_GOLD, "gold ore" },
	{ BlockType::BLOCK_ORE_IRON, "iron ore" },
	{ BlockType::BLOCK_SAND, "sand" },
	{ BlockType::BLOCK_STONE, "stone" },
	{ BlockType::BLOCK_STONE_BRICKS, "stonebricks" },
	{ BlockType::BLOCK_STONE_COBBLE, "cobblestone" },
	{ BlockType::BLOCK_STONE_CRACKED, "cracked stone" },
	{ BlockType::BLOCK_STONE_FRAMED, "framed stone" },
	{ BlockType::BLOCK_STONE_MOSSY, "mossy stone" },
	{ BlockType::BLOCK_TNT, "block of trinitrotoluene" },
	{ BlockType::BLOCK_WOOD, "wood" }
};

Block::Block(BlockType type)
	: type(type) {
}

std::string Block::toString() const {
	return blockNames.at(type);
}

bool Block::isSolid() {
	return !(type == BlockType::BLOCK_AIR || type == BlockType::BLOCK_WATER);
}
