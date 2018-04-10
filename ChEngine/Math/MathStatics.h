#ifndef MATH_STATICS
#define MATH_STATICS

#include "glm.h"

namespace Math {

	// returns a 2D rotation matrix, 0 degree points to the positive x direction
	static glm::mat2x2 Get2DRotationMat(const float degree)
	{
		glm::mat2x2 mat;
		mat[0] = glm::vec2(glm::cos(glm::radians(degree)), -glm::sin(glm::radians(degree)));
		mat[1] = glm::vec2(glm::sin(glm::radians(degree)), glm::cos(glm::radians(degree)));

		return mat;
	}

} // namespace Math

#endif
