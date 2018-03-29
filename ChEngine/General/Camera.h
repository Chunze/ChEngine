#pragma once

#include "glm.h"

enum class CameraType
{
	Camera_3D,
	Camera_2D
};

class Camera
{
public:
	Camera();

	// Type can be 3D or 2D, if 3D, next 4 floats will be fovy, aspect, zNear, and zFar;
	// If 2D, next 4 floats will be left, right, bottom, and top 
	Camera(CameraType type, float fovy_left, float aspect_right, float zNear_bottom, float zFar_top);

	CameraType m_type;

	glm::vec3 m_position;

	glm::vec3 m_right;
	glm::vec3 m_forward;
	glm::vec3 m_up;

	glm::vec3 m_flyDirection;

	glm::mat4 m_view;
	glm::mat4 m_perpective;

	float flySpeed = 20.0f;

	void SetupCamera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up);

	float pitch;
	float yaw;

	void FlyCameraForward(float value);

	void FlyCameraRight(float value);

	void FlyCameraUp(float value);

	void Fly(float deltaTime);

	void Rotate(float deltaPitch, float deltaYaw);

	void Update(float deltaTime);
};