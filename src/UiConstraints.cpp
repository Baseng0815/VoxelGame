#include "../include/UiConstraints.h"


Constraint::Constraint(ConstraintType type, float value)
    : type(type), value(value) {}

float Constraint::getValue(float parent) {
    switch (type) {
        case CONSTRAINT_RELATIVE:
            return parent * value;
        case CONSTRAINT_CENTER:
            return parent / 2.f;
        case CONSTRAINT_MATCH:
            return parent;

        default:
            return 0;
    }
}

#include <iostream>

Rectangle UiConstraints::getRect(const Rectangle& parent) {
    float w = width.getValue(parent.size.x);
    float h = height.getValue(parent.size.y);

    Rectangle rect(x.getValue(parent.size.x),
                         y.getValue(parent.size.y),
                         w, h);

    if (adjustPosition) {
        rect.position.x -= w / 2.f;
        rect.position.y -= h / 2.f;
    }

    return rect;
}
