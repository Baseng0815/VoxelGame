#pragma once

#include "Rectangle.h"

enum ConstraintType {
    CONSTRAINT_RELATIVE, CONSTRAINT_CENTER, CONSTRAINT_PIXEL, CONSTRAINT_MATCH
};

class Constraint {
    private:
        float value;
        ConstraintType type;

    public:
        Constraint(ConstraintType type = CONSTRAINT_RELATIVE, float value = 0);

        float getValue(float parent) const;
        ConstraintType getType() const;
};

#define RelativeConstraint(value)   Constraint(CONSTRAINT_RELATIVE, value)
#define CenterConstraint()          Constraint(CONSTRAINT_CENTER, 0)
#define PixelConstraint(value)      Constraint(CONSTRAINT_PIXEL, value)
#define MatchConstraint()           Constraint(CONSTRAINT_MATCH, 0)

class UiProperties;

struct UiConstraints {
    Constraint x = RelativeConstraint(0), y = RelativeConstraint(0),
               width = MatchConstraint(), height = MatchConstraint();

    // calculate the final area a widget occupies
    Rectangle getRect(const Rectangle& parent, const UiProperties& widgetProperties, int minWidth, int minHeight) const;
};
