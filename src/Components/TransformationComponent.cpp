#include "../../include/Components/TransformationComponent.h"

void TransformationComponent::recalculateModelMatrix() const {
    glm::mat4 rotationMatrix =
        glm::rotate(m_rotation.z, glm::vec3(0, 0, 1)) *
        glm::rotate(m_rotation.y, glm::vec3(0, 1, 0)) *
        glm::rotate(m_rotation.x, glm::vec3(1, 0, 0));

    // order of operations
    // - translate - scale - rotate -
    m_modelMatrix = glm::translate(m_position) * glm::scale(m_scale) * rotationMatrix;
}

TransformationComponent::TransformationComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {
        recalculateModelMatrix();
    }

const glm::vec3& TransformationComponent::getPosition() const {
    return m_position;
}

const glm::vec3& TransformationComponent::getRotation() const {
    return m_rotation;
}

const glm::vec3& TransformationComponent::getScale() const {
    return m_scale;
}

void TransformationComponent::setPosition(glm::vec3 position) {
    recalculateModelMatrix();
    m_position = position;
}

void TransformationComponent::setRotation(glm::vec3 rotation) {
    recalculateModelMatrix();
    m_rotation = rotation;
}

void TransformationComponent::setScale(glm::vec3 scale) {
    recalculateModelMatrix();
    m_scale = scale;
}

void TransformationComponent::move(glm::vec3 pdelta) {
    recalculateModelMatrix();
    m_position += pdelta;
}

void TransformationComponent::rotate(glm::vec3 protation) {
    recalculateModelMatrix();
    m_rotation += protation;
}

void TransformationComponent::scale(glm::vec3 pscale) {
    m_scale += pscale;
    recalculateModelMatrix();
}

glm::mat4 TransformationComponent::getModelMatrix() const {
    return m_modelMatrix;
}
