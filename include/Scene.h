#pragma once

#include "Player.h"
#include "World.h"
#include "Text.h"

struct Scene {
    Player player;
    World world;
    std::vector<Text*> text;
};
