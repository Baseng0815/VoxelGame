#include <glm/glm.hpp>


enum ConstraintType {
    CONSTRAINT_RELATIVE, CONSTRAINT_CENTER, CONSTRAINT_MATCH
};

class Constraint {
    private:
        float value;
        ConstraintType type;

    public:
        Constraint(ConstraintType type = CONSTRAINT_RELATIVE, float value = 0);
        float getValue(float parent);
};

#define RelativeConstraint(value)   Constraint(CONSTRAINT_RELATIVE, value)
#define CenterConstraint()          Constraint(CONSTRAINT_CENTER, 0)
#define MatchConstraint()           Constraint(CONSTRAINT_MATCH, 0)

struct UiConstraints {
    Constraint x = RelativeConstraint(0), y = RelativeConstraint(0),
               width = RelativeConstraint(1), height = RelativeConstraint(1);
    bool adjustPosition = false;

    Rectangle getRect(const Rectangle& parent);
};
