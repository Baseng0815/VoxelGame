#pragma once

#include "UiConstraints.h"

struct UiProperties {
    bool isVisible;
    UiConstraints constraints;
    glm::vec4 backgroundColor = glm::vec4(0);
};
