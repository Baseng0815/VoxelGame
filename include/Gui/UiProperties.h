#pragma once

#include "UiMargin.h"
#include "UiConstraints.h"

struct UiProperties {
    bool isVisible;
    UiConstraints constraints;
    Margin margin;
    glm::vec4 backgroundColor = glm::vec4(0);
};
