#pragma once
#include "glm.h"

class Light
{

public:
	Light() {}

	glm::vec3 m_position;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};