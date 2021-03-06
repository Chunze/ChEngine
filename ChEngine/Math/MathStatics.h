#ifndef MATH_STATICS
#define MATH_STATICS

#include "glm.h"

namespace Math {

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
		// Need to clamp the value, there might be rounding error which causes cosine to be out of [-1.0, 1.0]
		float _cosine = cosine;
		Clamp(_cosine, -1.0f, 1.0f);

		float sq_sum = sine * sine + _cosine * _cosine;

		assert(abs(sq_sum - 1.0f) < SMALL_NUMBER);

		// result of glm::acos is [0, PI]
		float result = glm::acos(_cosine) * 180.0f / PI;

		if (sine < 0)
		{
			result *= -1.0f;
		}

		return result;
	}

	// Get the rotation matrix out of a transformation matrix
	// This assumes that the transformation matrix has no scaling
	static mat3 GetRotationMatrix(const mat4 &Transform)
	{
		return mat3(Transform);
	}

	// Rotates a quaternion by a angular velocity with duration
	static quat Rotate(const quat &QuatToRotate, const vec3 &AngularVelocity, const float duration)
	{
		// quaternion constructed from angular velocity
		quat Result;
		quat q(0.0f, AngularVelocity.x, AngularVelocity.y, AngularVelocity.z);

		Result = QuatToRotate + duration * 0.5f * q * QuatToRotate;

		return glm::normalize(Result);
	}

	static float LengthSq(const vec3 &V)
	{
		return V.x * V.x + V.y * V.y + V.z * V.z;
	}

	// Generates orthonormal basis from 2 vectors (x and y)
	static bool MakeOrthonormalBasis(vec3 &x, vec3 &y, vec3&z)
	{
		x = glm::normalize(x);
		z = glm::cross(x, y);
		if (LengthSq(z) == 0.0f)
		{
			return false;
		}

		z = glm::normalize(z);
		y = glm::cross(z, x);

		return true;
	}

	static mat4 MakeTransform(const vec3 &Location, const quat &Orientation)
	{
		mat4 Result = glm::toMat4(glm::normalize(Orientation));
		Result[3] = vec4(Location, 1.0f);
		return Result;
	}

	static inline vec3 Multiply(const mat4 &mat, const vec3 &vec)
	{
		return vec3(mat * vec4(vec, 1.0f));
	}

} // namespace Math

#endif
