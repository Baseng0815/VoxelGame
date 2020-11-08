//#include "../../include/Gui/UiConstraints.hpp"

#include "../../include/Gui/UiProperties.hpp"

Constraint::Constraint(ConstraintType type, float value)
    : type(type), value(value) {
}

float Constraint::getPosition(float parentSize, float parentPosition) const {
    switch (type) {
        case ConstraintType::CONSTRAINT_RELATIVE:
            return parentSize * value;
        case ConstraintType::CONSTRAINT_CENTER:
            return parentPosition + parentSize / 2.f;
        case ConstraintType::CONSTRAINT_ABSOLUTE:
            return parentPosition + value;

        default:
            return 0;
    }
}

float Constraint::getSize(float parentSize) const {
    switch (type) {
        case ConstraintType::CONSTRAINT_RELATIVE:
            return parentSize * value;
        // case ConstraintType::CONSTRAINT_CENTER:
        //     return parentSize / 2.f;
        case ConstraintType::CONSTRAINT_ABSOLUTE:
            return value;

        default:
            return 0;
    }
}

ConstraintType Constraint::getType() const {
    return type;
}

Rectangle UiConstraints::getRect(const Rectangle& parent, const UiProperties& properties, const glm::vec2& minSize) const {
    float w, h;
    // handle match constraints
    if (width.getType() == ConstraintType::CONSTRAINT_MATCH)
        w = minSize.x;
    else
        w = width.getSize(parent.size.x);

    if (height.getType() == ConstraintType::CONSTRAINT_MATCH)
        h = minSize.y;
    else
        h = height.getSize(parent.size.y);

    Rectangle rect(x.getPosition(parent.size.x, parent.position.x),
                   y.getPosition(parent.size.y, parent.position.y),
                   w, h);

    // center if ConstraintType == CONSTRAINT_CENTER
    if (x.getType() == ConstraintType::CONSTRAINT_CENTER)
        rect.position.x -= w / 2;
    if (y.getType() == ConstraintType::CONSTRAINT_CENTER)
        rect.position.y -= h / 2;

    return rect;
}
