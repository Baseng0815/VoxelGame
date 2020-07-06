#include <glm/glm.hpp>

class Texture;

enum RenderMode {
    // renders using material color properties
    RENDER_COLOR,

    // renders using texture properties
    RENDER_TEXTURE
};

struct Material {
    // texture properties
    Texture* diffuseMap = nullptr;
    Texture* specularMap = nullptr;

    // color properties
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 ambient = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);
    float shininess = 1.0f;;

    RenderMode renderMode = RENDER_COLOR;
};
