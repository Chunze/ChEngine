#include "Camera.h"

Camera::Camera()
{
	// start position
	m_startPosition = vec3(3.0f, 3.0f, 3.0f);

	// Look at vector
	vec3 LookatPt = vec3(0.0f, 0.0f, 0.0f);		// looking at the origin
	m_startLookatDir = normalize(m_startPosition - LookatPt);

	// right vector
	vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
	m_startRightDir = normalize(cross(worldUp, m_startLookatDir));

	// up vector
	m_startUpDir = cross(m_startLookatDir, m_startRightDir);

	m_view = lookAt(m_startPosition, LookatPt, worldUp);
}
