#pragma once

#include "Window.h"

#include "Definitions.h"

enum ProjectionType {
    PROJECTION_PERSPECTIVE, PROJECTION_ORTHO
};

class Camera {
    private:
        float m_yaw = 0, m_pitch = 0, m_fov = 0;
        int m_width = Definitions::WINDOW_WIDTH, m_height = Definitions::WINDOW_HEIGHT;

        const float MOUSE_SENSITIVITY = 0.2f;
        const float MOUSE_SCROLL = 2.0f;
        const float MOVE_SPEED = 0.01f;

        glm::vec3 m_velocity;
        glm::vec3 m_position, m_up, m_right;
        glm::mat4 m_projectionPerspective, m_projectionOrtho, m_viewMatrix;


        // recalculate view matrix, front, up and right vectors
        // return new front vector
        glm::vec3 update(glm::vec3 position);

        void resize();

    public:
        Camera();

        void handleKey(int key, int action);
        void handleCursorPos(int dx, int dy);
        void handleScroll(double dy);
        void handleFramebufferSize(int width, int height);

        // dt in milliseconds
        void updatePosition(int dt);

        glm::mat4 getProjectionMatrix(ProjectionType type) const;
        glm::mat4 getViewMatrix() const;
        glm::vec3 getPosition() const;
};
