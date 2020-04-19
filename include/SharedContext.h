#include "Camera.h"
#include "TextureAtlas.h"

// a struct that is used to share data between systems without entities
struct SharedContext {
    Camera camera;
    TextureAtlas textureAtlas;

    SharedContext()
        : camera(90.f) {}
};