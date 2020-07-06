#include "../../include/Systems/System.h"
#include "../../include/Rendering/GBuffer.h"
#include "../../include/Rendering/RenderQuad.h"
#include "../../include/Rendering/Light.h"

struct Light {
    glm::vec3 dir;
    glm::vec3 color;
};

class Shader;
class Texture;

struct CameraComponent;
struct TransformationComponent;

struct FramebufferSizeEvent;

class MeshRenderSystem : public System {
    private:
        Shader* m_meshRenderShaderColor;
        Shader* m_meshRenderShaderTexture;
        Texture* m_atlas;

        PointLight light;

        void _update(int dt) override;

    public:
        MeshRenderSystem(entt::registry* registry);
};
