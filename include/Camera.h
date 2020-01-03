#pragma once

#include "Window.h"

#include "Definitions.h"

enum ProjectionType {
    PROJECTION_PERSPECTIVE, PROJECTION_ORTHO
};

class Event;

class Camera {
    private:
        float m_yaw, m_pitch, m_fov;
        int m_width = Definitions::WINDOW_WIDTH, m_height = Definitions::WINDOW_HEIGHT;

        const float MOUSE_SENSITIVITY = 0.2f;
        const float MOUSE_SCROLL = 2.0f;
        const float MOVE_SPEED = 0.01f;

        glm::vec3 m_up, m_right;
        glm::mat4 m_projectionPerspective, m_projectionOrtho, m_viewMatrix;

        // recalculate view matrix, front, up and right vectors
        // return new front vector
        glm::vec3 update(glm::vec3 position);

        void resize();

        void handleCursorPos(Event* event);
        void handleScroll(Event* event);
        void handleFramebufferSize(Event* event);

        friend class Player;

    public:
        Camera(float fov);

        glm::mat4 getProjectionMatrix(ProjectionType type) const;
        glm::mat4 getViewMatrix() const;
};
