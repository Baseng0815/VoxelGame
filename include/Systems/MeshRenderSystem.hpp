#include "../../include/Systems/System.hpp"
#include "../../include/Rendering/GBuffer.hpp"
#include "../../include/Rendering/RenderQuad.hpp"
#include "../../include/Rendering/Light.hpp"
#include "../../include/Resources/Shader.hpp"

class Shader;
class Texture;

struct CameraComponent;
struct CameraComponent;
struct MeshRenderComponent;
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

        void uploadToShader(const Shader *shader, const CameraComponent &camera) const;
        void render(const TransformationComponent &transformation, const MeshRenderComponent &meshRenderer, const CameraComponent &camera) const;

        void _update(int dt) override;

    public:
        MeshRenderSystem(Registry_T &registry);
};
