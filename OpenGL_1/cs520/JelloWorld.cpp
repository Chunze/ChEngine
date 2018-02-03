#define _CRT_SECURE_NO_WARNINGS
#include <fstream>

#include "JelloWorld.h"
#include "Jello.h"
#include "PhysicsManager.h"


JelloWorld::JelloWorld(GameContext gameContext)
	: World(gameContext)
{
	m_jello = new Jello(m_gameContext, this, glm::vec3(0.0f, 0.0f, 0.0));
	m_gameObjects.push_back(*m_jello);
}

void JelloWorld::InitWorld()
{
	
}

void JelloWorld::LoadWorld(const char* fileName)
{
	int i, j, k;
	FILE * file;

	fopen_s(&file, fileName, "r");
	if (file == NULL) {
		printf("can't open file\n");
		exit(1);
	}

	/* read integrator algorithm */
	fscanf(file, "%s\n", &m_intergrator);
	if (m_intergrator[0] == 'E')
	{
		m_gameContext.GetPhysicsManager()->SetIntegrator(0);
	}
	else if (m_intergrator[0] == 'R')
	{
		m_gameContext.GetPhysicsManager()->SetIntegrator(1);
	}

	/* read timestep size and render */
	fscanf_s(file, "%f %d\n", &m_dt, &m_n);

	/* read physical parameters */
	fscanf_s(file, "%f %f %f %f\n",
		&m_jello->m_kElastic, &m_jello->m_dElastic, &m_jello->m_kCollision, &m_jello->m_dCollision);

	/* read mass of each of the 512 points */
	fscanf_s(file, "%f\n", &m_jello->m_mass);

	/* read info about the plane */
	fscanf_s(file, "%d\n", &m_incPlanePresent);
	if (m_incPlanePresent == 1)
	{
		fscanf_s(file, "%f %f %f %f\n", &a, &b, &c, &d);
	}

	/* read info about the force field */
	fscanf_s(file, "%d\n", &m_resolution);
	m_forceField = (glm::vec3*)malloc(m_resolution * m_resolution * m_resolution * sizeof(glm::vec3));

	if (m_resolution != 0)
		for (i = 0; i <= m_resolution - 1; i++)
			for (j = 0; j <= m_resolution - 1; j++)
				for (k = 0; k <= m_resolution - 1; k++)
					fscanf_s(file, "%f %f %f\n",
						&m_forceField[i * m_resolution * m_resolution + j * m_resolution + k].x,
						&m_forceField[i * m_resolution * m_resolution + j * m_resolution + k].y,
						&m_forceField[i * m_resolution * m_resolution + j * m_resolution + k].z);

	/* read initial point positions */
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			for (k = 0; k <= 7; k++)
				fscanf_s(file, "%f %f %f\n",
					&m_jello->m_particles[i][j][k].m_position.x, 
					&m_jello->m_particles[i][j][k].m_position.y,
					&m_jello->m_particles[i][j][k].m_position.z);
		}
	}

	/* read initial point velocities */
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			for (k = 0; k <= 7; k++)
				fscanf_s(file, "%f %f %f\n",
					&m_jello->m_particles[i][j][k].m_volecity.x,
					&m_jello->m_particles[i][j][k].m_volecity.y,
					&m_jello->m_particles[i][j][k].m_volecity.z);
		}
	}

	fclose(file);
}

void JelloWorld::Update(float Delta)
{
	m_jello->Update(Delta);
}

