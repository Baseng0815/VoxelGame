#include "../../include/GameData/Block.hpp"

#include "../../include/GameData/GameData.hpp"

bool Block::isSolid() const {
    return GameData::getBlockTemplate(type).isSolid;
}

bool Block::isTransparent() const {
    return GameData::getBlockTemplate(type).isTransparent;
}
