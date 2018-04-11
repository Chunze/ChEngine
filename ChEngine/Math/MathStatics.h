#ifndef MATH_STATICS
#define MATH_STATICS

#include "glm.h"

namespace Math {

#define SMALL_NUMBER	1e-6
#define PI				3.14159265359

	static void ClampAngle0To360(float& angle)
	{
		while (angle >= 360.0f)
		{
			angle -= 360.0f;
		}

		while (angle < 0)
		{
			angle += 360.0f;
		}
	}

	static void ClampAngle180(float& angle)
	{
		while (angle >= 180.0f)
		{
			angle -= 360.0f;
		}

		while (angle < -180.0f)
		{
			angle += 360.0f;
		}
	}

	static void Clamp(float& target, const float clamp_min, const float clamp_max)
	{
		if (target < clamp_min)
		{
			target = clamp_min;
		}
		else if (target > clamp_max)
		{
			target = clamp_max;
		}
	}

	// returns a 2D rotation matrix, 0 degree points to the positive x direction
	static glm::mat2x2 Get2DRotationMat(const float degree)
	{
		glm::mat2x2 mat;
		mat[0] = glm::vec2(glm::cos(glm::radians(degree)), -glm::sin(glm::radians(degree)));
		mat[1] = glm::vec2(glm::sin(glm::radians(degree)), glm::cos(glm::radians(degree)));

		return mat;
	}

	static float Cross(const glm::vec2 v1, const glm::vec2 v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	static float GetAngleInDegree(const float sine, const float cosine)
	{
		float sq_sum = sine * sine + cosine * cosine;

		assert(abs(sq_sum - 1.0f) < SMALL_NUMBER);

		// result of glm::acos is [0, PI]
		float result = glm::acos(cosine) * 180.0f / PI;

		if (sine < 0)
		{
			result *= -1.0f;
		}

		return result;
	}
} // namespace Math

#endif
