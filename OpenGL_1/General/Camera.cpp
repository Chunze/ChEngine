#include <GLFW/glfw3.h>
#include "Camera.h"

Camera::Camera()
{
	// start position
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);

	// Look at vector
	glm::vec3 LookatPt = glm::vec3(0.0f, 0.0f, 0.0f);		// looking at the origin
	m_forward = normalize(LookatPt - m_position);

	// right vector
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = normalize(cross(m_forward, worldUp));

	// up vector
	m_up = normalize(cross(m_right, m_forward));

	m_view = lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::FlyCameraForward(float value)
{
	m_position += flySpeed * value * m_forward;
}

void Camera::FlyCameraRight(float value)
{
	m_position += flySpeed * value * m_right;
}

void Camera::Update()
{
	m_view = lookAt(m_position, m_position + m_forward, m_up);
}
