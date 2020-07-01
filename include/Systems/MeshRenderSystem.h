#include "../../include/Systems/System.h"
#include "../../include/Rendering/GBuffer.h"
#include "../../include/Rendering/RenderQuad.h"

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
        // first render pass
        Shader* m_meshRenderShader;
        Texture* m_atlas;

        void _update(int dt) override;

    public:
        MeshRenderSystem(entt::registry* registry);
};
