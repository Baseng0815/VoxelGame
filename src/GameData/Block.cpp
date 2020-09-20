#include "../../include/GameData/Block.h"

#include "../../include/GameData/GameData.h"

bool Block::isSolid()
{
    return GameData::getBlockTemplate(type).isSolid;
}
