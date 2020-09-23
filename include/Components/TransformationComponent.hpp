#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

struct TransformationComponent {
    private:
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        void recalculateModelMatrix() const;
        mutable glm::mat4 m_modelMatrix;

    public:
        TransformationComponent(glm::vec3 position = glm::vec3(0.0f), glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f));

        const glm::vec3 &getPosition() const;
        const glm::quat &getRotation() const;
        const glm::vec3 &getScale() const;

        void setPosition(const glm::vec3&);
        void setRotation(const glm::quat&);
        void setScale(const glm::vec3&);

        void move(const glm::vec3&);
        void rotate(const glm::quat&);
        void scale(const glm::vec3&);

        const glm::mat4 &getModelMatrix() const;
};
