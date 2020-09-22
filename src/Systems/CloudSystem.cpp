#include "../../include/Systems/CloudSystem.h"

#include "../../include/Events/EventDispatcher.h"
#include "../../include/Configuration.h"
#include "../../include/Resources/ResourceManager.h"

#include "../../include/Components/MeshRenderComponent.h"
#include "../../include/Components/TransformationComponent.h"

#include <ctime>

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

void CloudSystem::handleEnterChunk(const EnterChunkEvent &e)
{
    m_chunkPosX = e.newX;
    m_chunkPosZ = e.newZ;
    recalculateCloudGeometry();
}

void CloudSystem::recalculateCloudGeometry()
{
    const bool hasVolume = Configuration::getFloatValue("VOLUMINOUS_CLOUDS") || true;
    // cloud geometry is generated on a per-chunk basis as a single mesh
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    try {
        if (!hasVolume) {
            vertices.reserve(Configuration::CHUNK_COUNT_PER_AXIS * Configuration::CHUNK_COUNT_PER_AXIS * 4);
            indices.reserve(Configuration::CHUNK_COUNT_PER_AXIS * Configuration::CHUNK_COUNT_PER_AXIS * 6);
        } else {
            vertices.reserve(Configuration::CHUNK_PRELOAD_SIZE * Configuration::CHUNK_PRELOAD_SIZE * 4 * 2);
            indices.reserve(Configuration::CHUNK_COUNT_PER_AXIS * Configuration::CHUNK_COUNT_PER_AXIS * 6 * 6);
        }
    } catch (std::length_error e) {
        std::cout << "WARNING: cloud buffer preallocation failed; " << e.what() << std::endl;
    }

    int cloudIndex = 0;
    // TODO optimize overlapping vertices etc. etc. if bored
    for (int chunkX = -Configuration::CHUNK_PRELOAD_SIZE; chunkX <= Configuration::CHUNK_PRELOAD_SIZE; chunkX++) {
        for (int chunkZ = -Configuration::CHUNK_PRELOAD_SIZE; chunkZ <= Configuration::CHUNK_PRELOAD_SIZE; chunkZ++) {
            const int worldChunkX = chunkX + m_chunkPosX;
            const int worldChunkZ = chunkZ + m_chunkPosZ;
            const bool isCloud = m_cloudModule.GetValue(worldChunkX, worldChunkZ - m_chunkOffset);
            if (isCloud) {
                const float left    = worldChunkX * Configuration::CHUNK_SIZE;
                const float right   = (worldChunkX + 1) * Configuration::CHUNK_SIZE;
                const float front   = worldChunkZ * Configuration::CHUNK_SIZE;
                const float back    = (worldChunkZ - 1) * Configuration::CHUNK_SIZE;

                // cloud geometry on height 0; raised through model matrix
                vertices.emplace_back(Vertex {glm::vec3 {left, 0.f, front}});
                vertices.emplace_back(Vertex {glm::vec3 {left, 0.f, back}});
                vertices.emplace_back(Vertex {glm::vec3 {right, 0.f, back}});
                vertices.emplace_back(Vertex {glm::vec3 {right, 0.f, front}});

                if (hasVolume) {
                    // add the other four vertices
                    vertices.emplace_back(Vertex {glm::vec3 {left, Configuration::CLOUD_VOLUMETRIC_HEIGHT, front}});
                    vertices.emplace_back(Vertex {glm::vec3 {left, Configuration::CLOUD_VOLUMETRIC_HEIGHT, back}});
                    vertices.emplace_back(Vertex {glm::vec3 {right, Configuration::CLOUD_VOLUMETRIC_HEIGHT, back}});
                    vertices.emplace_back(Vertex {glm::vec3 {right, Configuration::CLOUD_VOLUMETRIC_HEIGHT, front}});

                    constexpr unsigned int offsetsVolume[] = {
                        // -Y
                        0, 1, 2, 0, 2, 3,
                        // +Y
                        4, 5, 6, 4, 6, 7,
                        // +Z
                        0, 4, 7, 0, 7, 3,
                        // -X
                        1, 5, 4, 1, 4, 0,
                        // -Z
                        /* // this will only be added when the Z- neighbor is empty */
                        2, 6, 5, 2, 5, 1,
                        // +X
                        /* // this will only be added when the X+ neighbor is empty */
                        3, 7, 6, 3, 6, 2,
                    };

                    for (size_t i = 0; i < 24; i++) {
                        indices.emplace_back(cloudIndex * 8 + offsetsVolume[i]);
                    }

                    const bool neighborXIsCloud = m_cloudModule.GetValue((worldChunkX + 1), worldChunkZ - m_chunkOffset);
                    if (!(neighborXIsCloud && worldChunkX + 1 <= Configuration::CHUNK_PRELOAD_SIZE)) {
                        for (size_t i = 24; i < 30; i++) {
                            indices.emplace_back(cloudIndex * 8 + offsetsVolume[i]);
                        }
                    }

                    const bool neighborZIsCloud = m_cloudModule.GetValue(worldChunkX, (worldChunkZ - 1) - m_chunkOffset);
                    if (!(neighborZIsCloud && worldChunkZ - 1 >= Configuration::CHUNK_PRELOAD_SIZE)) {
                        for (size_t i = 30; i < 36; i++) {
                            indices.emplace_back(cloudIndex * 8 + offsetsVolume[i]);
                        }
                    }
                } else {
                    constexpr unsigned int offsetsNoVolume[] = { 0, 1, 2, 0, 2, 3 };

                    for (size_t i = 0; i < 6; i++) {
                        indices.emplace_back(cloudIndex * 4 + offsetsNoVolume[i]);
                    }
                }

                cloudIndex++;
            }
        }
    }

    m_geometry.fillBuffers(vertices, indices);
    // TODO when implementing this, change if statement and for loop
}

void CloudSystem::_update(int dt)
{
    TransformationComponent &transform = m_registry.get<TransformationComponent>(m_cloudEntity);

    transform.move(glm::vec3 {0.f, 0.f, (dt / 1000.f) * Configuration::CLOUD_SPEED});
    // recalculate cloud geometry because they passed a chunk
    if (transform.getPosition().z >= Configuration::CHUNK_SIZE) {
        m_chunkOffset += (int)(transform.getPosition().z / Configuration::CHUNK_SIZE);
        transform.setPosition(glm::vec3 {0.f, (float)Configuration::CLOUD_HEIGHT, 0.f});
        recalculateCloudGeometry();
    }
}

CloudSystem::CloudSystem(Registry_T &registry)
    : System {registry, 0}, m_cloudEntity {registry.create()}
{
    // all clouds are represented by a single entity and rendered using the mesh renderer
    registry.emplace<MeshRenderComponent>(m_cloudEntity, ResourceManager::getResource<Material>(MATERIAL_CLOUDS), &m_geometry);
    registry.emplace<TransformationComponent>(m_cloudEntity, glm::vec3 {0.f, Configuration::CLOUD_HEIGHT, 0.f});

    m_perlinModule.SetFrequency(8.5);
    m_perlinModule.SetOctaveCount(1);
    m_perlinModule.SetSeed(std::time(NULL));
    m_cloudModule.SetSourceModule(0, m_perlinModule);

    recalculateCloudGeometry();

    m_enterChunkHandle = EventDispatcher::onEnterChunk.subscribe([&](const EnterChunkEvent &e) {
        handleEnterChunk(e);
    });
}
