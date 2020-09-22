#include "../../include/Math/Face.hpp"
#include "../../include/Math/Ray.hpp"

namespace Math {

	Face::Face(std::initializer_list<glm::vec3> corners) : corners(corners) {
	}

	/*bool Face::intersects(const Ray& ray, float* t) const {

	}*/
}