#include "../include/UiConstraints.h"


Constraint::Constraint(ConstraintType type, float value)
    : type(type), value(value) {}

float Constraint::getValue(float parent) const {
    switch (type) {
        case CONSTRAINT_RELATIVE:
            return parent * value;
        case CONSTRAINT_CENTER:
            return parent / 2.f;
        case CONSTRAINT_PIXEL:
            return value;
        case CONSTRAINT_MATCH:
            return 0;

        default:
            return 0;
    }
}

ConstraintType Constraint::getType() const {
    return type;
}

Rectangle UiConstraints::getRect(const Rectangle& parent) const {
    float w = width.getValue(parent.size.x);
    float h = height.getValue(parent.size.y);

    Rectangle rect(x.getValue(parent.size.x),
                         y.getValue(parent.size.y),
                         w, h);

    return rect;
}
