#include "../../include/Systems/CloudSystem.h"

#include "../../include/Configuration.h"
#include "../../include/Resources/ResourceManager.h"

#include "../../include/Components/MeshRenderComponent.h"
#include "../../include/Components/TransformationComponent.h"

CloudModule::CloudModule()
    : module::Module { GetSourceModuleCount() }
{  }

int CloudModule::GetSourceModuleCount() const
{ return 1; }

double CloudModule::GetValue(double x, double y, double z) const
{
    double val = m_pSourceModule[0]->GetValue(x, y, z);
    /* return val; */
    if (val < 0.0) return -1.0;
    else return 1.0;
}

#include <iostream>

void CloudSystem::calculateCloudGeometry()
{
    bool hasVolume = Configuration::getFloatValue("VOLUMINOUS_CLOUDS");
    // cloud geometry is generated on a per-chunk basis and then merged into a single mesh renderer
    std::vector<Vertex> vertices;

    // reserve some space to prevent reallocations
    if (!hasVolume) {
        vertices.reserve(Configuration::CHUNK_PRELOAD_SIZE * Configuration::CHUNK_PRELOAD_SIZE * 4);
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
        calculateCloudGeometry();
    }
}

CloudSystem::CloudSystem(Registry_T &registry)
    : System {registry, 10}, m_cloudEntity {registry.create()}
{
    // all clouds are represented by a single entity and rendered using the mesh renderer
    registry.emplace<MeshRenderComponent>(m_cloudEntity, MeshRenderComponent {ResourceManager::getResource<Material>(MATERIAL_CLOUDS)});
    registry.emplace<TransformationComponent>(m_cloudEntity, TransformationComponent {glm::vec3 {0.f, Configuration::CLOUD_HEIGHT, 0.f}});
}
