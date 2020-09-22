#pragma once

#include "Rectangle.hpp"

enum class ConstraintType {
    CONSTRAINT_RELATIVE, CONSTRAINT_CENTER, CONSTRAINT_ABSOLUTE, CONSTRAINT_MATCH
};

class Constraint {
    private:
        float value;
        ConstraintType type;

    public:
        Constraint(ConstraintType type = ConstraintType::CONSTRAINT_RELATIVE, float value = 0);

        float getValue(float parent) const;
        ConstraintType getType() const;
};

#define RelativeConstraint(value)   Constraint(ConstraintType::CONSTRAINT_RELATIVE, value)
#define CenterConstraint()          Constraint(ConstraintType::CONSTRAINT_CENTER, 0)
#define AbsoluteConstraint(value)   Constraint(ConstraintType::CONSTRAINT_ABSOLUTE, value)
#define MatchConstraint()           Constraint(ConstraintType::CONSTRAINT_MATCH, 0)

class UiProperties;

struct UiConstraints {
    Constraint x = RelativeConstraint(0), y = RelativeConstraint(0),
               width = MatchConstraint(), height = MatchConstraint();

    // calculate the final area a widget occupies
    Rectangle getRect(const Rectangle& parent, const UiProperties& widgetProperties, const glm::vec2 &minSize) const;
};
