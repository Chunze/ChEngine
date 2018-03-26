#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();

	glm::vec3 m_position;

	glm::vec3 m_right;
	glm::vec3 m_forward;
	glm::vec3 m_up;

	glm::vec3 m_flyDirection;

	glm::mat4 m_view;

	float flySpeed = 20.0f;

	float pitch;
	float yaw;

	void FlyCameraForward(float value);

	void FlyCameraRight(float value);

	void FlyCameraUp(float value);

	void Fly(float deltaTime);

	void Rotate(float deltaPitch, float deltaYaw);

	void Update(float deltaTime);
};