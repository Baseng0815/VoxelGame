#include "../../include/Systems/System.h"
#include "../../include/Rendering/GBuffer.h"
#include "../../include/Rendering/RenderQuad.h"
#include "../../include/Rendering/Light.h"
#include "../../include/Resources/Shader.h"

class Shader;
class Texture;

struct CameraComponent;
struct TransformationComponent;

struct FramebufferSizeEvent;

// TODO add light components instead of hard coding
#define MAX_LIGHTS 1

class MeshRenderSystem : public System {
    private:
        // TODO maybe use dynamically each frame instead of storing it here
        const Shader *m_meshRenderShaderColor;
        const Shader *m_meshRenderShaderTexture;

        PointLight m_pointLights[MAX_LIGHTS];
        DirectionalLight m_sun;

        void _update(int dt) override;

    public:
        MeshRenderSystem(entt::registry* registry);
};
