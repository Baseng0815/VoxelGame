#include "TextureAtlas.h"

// TODO remove this stupid thing
struct SharedContext {
    TextureAtlas textureAtlas;

    // TODO introduce nice resource manager
    SharedContext()
        : textureAtlas("Resources/Textures/textureAtlas0.png") {}
};
