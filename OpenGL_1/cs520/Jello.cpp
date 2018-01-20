#include "Jello.h"

Jello::Jello()
{

}

Jello::Jello(glm::vec3 position, float size)
	: m_position(position),
	  m_size(size)
{
	float step = m_size / 7.0f;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				glm::vec3 particlePos = glm::vec3(m_position.x + i * step, m_position.y + j * step, m_position.z + k * step);
				Particle* temp = new Particle(1.0f, particlePos, glm::vec3(0.0));
				m_particles[i][j][k] = *temp;
			}
		}
	}
}

