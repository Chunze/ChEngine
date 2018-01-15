#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Camera
{
public:
	Camera();

	vec3 m_startPosition;

	vec3 m_startRightDir;
	vec3 m_startLookatDir;
	vec3 m_startUpDir;

	mat4 m_view;
};