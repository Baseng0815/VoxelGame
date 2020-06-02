#include "../../include/Gui/UiConstraints.h"

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

        default:
            return 0;
    }
}

ConstraintType Constraint::getType() const {
    return type;
}

Rectangle UiConstraints::getRect(const Rectangle& parent, int minWidth, int minHeight) const {
    float w, h;
    // handle match constraints
    if (width.getType() == CONSTRAINT_MATCH)
        w = minWidth;
    else
        w = width.getValue(parent.size.x);

    if (height.getType() == CONSTRAINT_MATCH)
        h = minHeight;
    else
        h = height.getValue(parent.size.y);

    Rectangle rect(x.getValue(parent.size.x),
                         y.getValue(parent.size.y),
                         w, h);

    // center if ConstraintType == CONSTRAINT_CENTER
    if (x.getType() == CONSTRAINT_CENTER)
        rect.position.x -= w / 2;
    if (y.getType() == CONSTRAINT_CENTER)
        rect.position.y -= h / 2;

    return rect;
}
