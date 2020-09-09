#include "../../include/Systems/CloudSystem.h"

#include "../../include/Resources/ResourceManager.h"
#include "../../include/Configuration.h"

#include "../../include/Components/MeshRenderComponent.h"
#include "../../include/Components/TransformationComponent.h"

CloudModule::CloudModule()
    : module::Module { GetSourceModuleCount() }
{ }

int CloudModule::GetSourceModuleCount() const
{ return 1; }

double CloudModule::GetValue(double x, double y, double z) const
{
    double val = m_pSourceModule[0]->GetValue(x, y, z);
    if (val < Configuration::CLOUD_BIAS) return 1.0;
    else return 0.0;
}

void CloudSystem::recalculateCloudGeometry()
{
    const bool hasVolume = Configuration::getFloatValue("VOLUMINOUS_CLOUDS");
    // cloud geometry is generated on a per-chunk basis and then merged into a single mesh renderer
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    int cloudIndex = 0;
    if (!hasVolume) {
        vertices.reserve(Configuration::CHUNK_COUNT_PER_AXIS * Configuration::CHUNK_COUNT_PER_AXIS * 4);
        indices.reserve(Configuration::CHUNK_COUNT_PER_AXIS * Configuration::CHUNK_COUNT_PER_AXIS * 6);
        // TODO add position and onEnterChunk handle
        for (int chunkX = -Configuration::CHUNK_PRELOAD_SIZE; chunkX <= Configuration::CHUNK_PRELOAD_SIZE; chunkX++) {
            for (int chunkZ = -Configuration::CHUNK_PRELOAD_SIZE; chunkZ <= Configuration::CHUNK_PRELOAD_SIZE; chunkZ++) {
                const bool isCloud = m_cloudModule.GetValue(chunkX, chunkZ - m_chunkOffset);
                if (isCloud) {
                    const int left    = chunkX * Configuration::CHUNK_SIZE;
                    const int right   = (chunkX + 1) * Configuration::CHUNK_SIZE;
                    const int front   = chunkZ * Configuration::CHUNK_SIZE;
                    const int back    = (chunkZ - 1) * Configuration::CHUNK_SIZE;

                    // cloud geometry on height 0; raised through model matrix
                    vertices.emplace_back(Vertex {glm::vec3 {left, 0, front}});
                    vertices.emplace_back(Vertex {glm::vec3 {left, 0, back}});
                    vertices.emplace_back(Vertex {glm::vec3 {right, 0, back}});
                    vertices.emplace_back(Vertex {glm::vec3 {right, 0, front}});

                    constexpr unsigned int offsets[] = { 0, 1, 2, 0, 2, 3 };

                    for (int i = 0; i < 6; i++) {
                        indices.emplace_back(cloudIndex * 4 + offsets[i]);
                    }

                    cloudIndex++;
                }
            }
        }

        MeshRenderComponent &meshRenderer = m_registry.get<MeshRenderComponent>(m_cloudEntity);
        meshRenderer.geometry.fillBuffers(vertices, indices);

    // TODO when implementing this, change if statement and for loop
    } else {

        vertices.reserve(Configuration::CHUNK_PRELOAD_SIZE * Configuration::CHUNK_PRELOAD_SIZE * 4);
    }
}

void CloudSystem::_update(int dt)
{
    TransformationComponent &transform = m_registry.get<TransformationComponent>(m_cloudEntity);

    transform.move(glm::vec3 {0.f, 0.f, (dt / 1000.f) * Configuration::CLOUD_SPEED});
    // recalculate cloud geometry because they passed a chunk
    if (transform.getPosition().z > Configuration::CHUNK_SIZE) {
        m_chunkOffset += (int)(transform.getPosition().z / Configuration::CHUNK_SIZE);
        transform.setPosition(glm::vec3 {0.f, (float)Configuration::CLOUD_HEIGHT, 0.f});
        recalculateCloudGeometry();
    }
}

CloudSystem::CloudSystem(Registry_T &registry)
    : System {registry, 0}, m_cloudEntity {registry.create()}
{
    // all clouds are represented by a single entity and rendered using the mesh renderer
    registry.emplace<MeshRenderComponent>(m_cloudEntity, MeshRenderComponent {ResourceManager::getResource<Material>(MATERIAL_CLOUDS)});
    registry.emplace<TransformationComponent>(m_cloudEntity, TransformationComponent {glm::vec3 {0.f, Configuration::CLOUD_HEIGHT, 0.f}});

    m_perlinModule.SetFrequency(8.5);
    m_perlinModule.SetOctaveCount(1);
    m_perlinModule.SetSeed(time(NULL));
    m_cloudModule.SetSourceModule(0, m_perlinModule);

    recalculateCloudGeometry();
}
