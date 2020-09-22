#include "../../include/GameData/Block.hpp"

#include "../../include/GameData/GameData.hpp"

bool Block::isSolid()
{
    return GameData::getBlockTemplate(type).isSolid;
}
