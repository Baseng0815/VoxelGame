//#include "../../include/Gui/UiConstraints.hpp"

#include "../../include/Gui/UiProperties.hpp"

Constraint::Constraint(ConstraintType type, float value)
    : type(type), value(value) {}

float Constraint::getValue(float parentSize, float parentPosition) const {
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

ConstraintType Constraint::getType() const {
    return type;
}

Rectangle UiConstraints::getRect(const Rectangle& parent, const UiProperties& properties, const glm::vec2 &minSize) const {
    float w, h;
    // handle match constraints
    if (width.getType() == ConstraintType::CONSTRAINT_MATCH)
        w = minSize.x;
    else
        w = width.getValue(parent.size.x, parent.position.x);

    if (height.getType() == ConstraintType::CONSTRAINT_MATCH)
        h = minSize.y;
    else
        h = height.getValue(parent.size.y, parent.position.y);

    Rectangle rect(x.getValue(parent.size.x, parent.position.x),
                         y.getValue(parent.size.y, parent.position.y),
                         w, h);

    // center if ConstraintType == CONSTRAINT_CENTER
    if (x.getType() == ConstraintType::CONSTRAINT_CENTER)
        rect.position.x -= w / 2;
    if (y.getType() == ConstraintType::CONSTRAINT_CENTER)
        rect.position.y -= h / 2;

    return rect;
}
