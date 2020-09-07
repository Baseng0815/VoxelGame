#include "../../include/Math/Face.h"
#include "../../include/Math/Ray.h"

Face::Face(std::initializer_list<glm::vec3> corners) : corners(corners) {
}

/*bool Face::intersects(const Ray& ray, float* t) const {

}*/