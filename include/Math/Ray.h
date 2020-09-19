#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Math {
	struct Plane;

	struct Ray {
	private:
		glm::vec3 direction;

	public:
		glm::vec3 origin;
		Ray();

		Ray(glm::vec3 origin, glm::vec3 direction);

		glm::vec3 getPoint(float t) const;
		glm::vec3 getDirection() const;
		void setDirection(const glm::vec3 dir);

		bool intersects(const Plane& plane, float* t) const;

		template <typename type> glm::vec3 getFirstBlock(float maxT, type predicate) const {
			float t = 0;
			glm::vec3 position;

			float stepX = glm::abs(1 / direction.x);
			float stepY = glm::abs(1 / direction.y);
			float stepZ = glm::abs(1 / direction.z);

			float tx = 0;
			float ty = 0;
			float tz = 0;

			do {
				float tmin = glm::min(tx, glm::min(ty, tz));

				if (tmin == tx) {
					t = tx;
					tx += stepX;
				}

				if (tmin == ty) {
					t = ty;
					ty += stepY;
				}

				if (tmin == tz) {
					t = tz;
					tz += stepZ;
				}

				position = glm::floor(getPoint(t));
			} while (glm::abs(t) <= maxT && !predicate(position));

			if (t > maxT) {
				return glm::vec3(0, -1, 0);
			}
			else {
				return position;
			}
		}
	};
}