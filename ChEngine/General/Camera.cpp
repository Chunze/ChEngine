#include <GLFW/glfw3.h>
#include "Camera.h"

#include <iostream>

Camera::Camera()
{
	m_flyDirection = glm::vec3(0.0f);
}

Camera::Camera(CameraType type, float fovy, float aspect, float zNear, float zFar)
{

	m_type = type;
	if (type == CameraType::Camera_3D)
	{
		m_perpective = glm::perspective(fovy, aspect, zNear, zFar);
	}
	else if (type == CameraType::Camera_2D)
	{
		m_perpective = glm::ortho(fovy, aspect, zNear, zFar);
	}
}

void Camera::SetupCamera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up)
{
	m_position = position;

	glm::vec3 LookatPt = lookat;
	m_forward = glm::normalize(LookatPt - m_position);

	m_up = up;

	m_view = lookAt(m_position, m_position + m_forward, m_up);


	// initializing pitch and yaw based on forward, so that rotating the camera works fine.
	pitch = glm::degrees(glm::asin(m_forward.y));

	// some wired angle conversion
	if (m_forward.x < 0)
	{
		yaw = glm::degrees(glm::atan(m_forward.z / m_forward.x));
		yaw += m_forward.z > 0 ? 180.0f : -180.0f;
	}
}

void Camera::FlyCameraForward(float value)
{
	//m_position += flySpeed * value * m_forward;

	m_flyDirection += value * m_forward;		// positive z is pointing out of the screen
}

void Camera::FlyCameraRight(float value)
{
	//m_position += flySpeed * value * m_right;
	m_flyDirection += value * m_right;
}

void Camera::FlyCameraUp(float value)
{
	//m_position += flySpeed * value * m_up;
	m_flyDirection += value * m_up;
}

/* This function ensures that the camera is flying in constant speed */
void Camera::Fly(float deltaTime)
{
	if (m_flyDirection == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		return;
	}

	m_position += 0.015f * flySpeed * glm::normalize(m_flyDirection);
}

void Camera::Rotate(float deltaPitch, float deltaYaw)
{
	pitch += deltaPitch;
	yaw += deltaYaw;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	m_forward = glm::normalize(front);
	m_right = glm::cross(m_forward, m_up);

	//std::cout << "front = " << "(" << m_forward.x << "," << m_forward.y << "," << m_forward.z << ")" << std::endl;

	//m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	// up vector
	//m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void Camera::Update(float deltaTime)
{
	Fly(deltaTime);
	m_view = lookAt(m_position, m_position + m_forward, m_up);

	m_flyDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::UpdatePerspective(float f1, float f2, float f3, float f4)
{
// 	if (m_type == CameraType::Camera_3D)
// 	{
// 		m_perpective = glm::perspective(f1, f2, f3, f4);
// 	}
	if (m_type == CameraType::Camera_2D)
	{
		m_perpective = glm::ortho(f1, f2, f3, f4);
	}
}
