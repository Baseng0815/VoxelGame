#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct TransformationComponent {
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        void recalculateModelMatrix() const;
        mutable glm::mat4 m_modelMatrix;

    public:
        TransformationComponent(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

        const glm::vec3& getPosition() const;
        const glm::vec3& getRotation() const;
        const glm::vec3& getScale() const;

        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation);
        void setScale(glm::vec3 scale);

        void move(glm::vec3 pdelta);
        void rotate(glm::vec3 protation);
        void scale(glm::vec3 pscale);

        glm::mat4 getModelMatrix() const;
};
