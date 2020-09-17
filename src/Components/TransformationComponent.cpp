#include "../../include/Components/TransformationComponent.h"

void TransformationComponent::recalculateModelMatrix() const {
    glm::mat4 rotationMatrix = glm::toMat4(m_rotation);

    // order of operations
    // - translate - scale - rotate -
    m_modelMatrix = glm::translate(m_position) * glm::scale(m_scale) * rotationMatrix;
}

TransformationComponent::TransformationComponent(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {
        recalculateModelMatrix();
    }

const glm::vec3 &TransformationComponent::getPosition() const {
    return m_position;
}

const glm::quat &TransformationComponent::getRotation() const {
    return m_rotation;
}

const glm::vec3 &TransformationComponent::getScale() const {
    return m_scale;
}

void TransformationComponent::setPosition(const glm::vec3 &position) {
    recalculateModelMatrix();
    m_position = position;
}

void TransformationComponent::setRotation(const glm::quat &rotation) {
    recalculateModelMatrix();
    m_rotation = rotation;
}

void TransformationComponent::setScale(const glm::vec3 &scale) {
    recalculateModelMatrix();
    m_scale = scale;
}

void TransformationComponent::move(const glm::vec3 &pdelta) {
    m_position += pdelta;
    recalculateModelMatrix();
}

void TransformationComponent::rotate(const glm::quat &protation) {
    m_rotation *= protation;
    recalculateModelMatrix();
}

void TransformationComponent::scale(const glm::vec3 &pscale) {
    m_scale += pscale;
    recalculateModelMatrix();
}

glm::mat4 TransformationComponent::getModelMatrix() const {
    return m_modelMatrix;
}
