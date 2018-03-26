#include <fstream>
#include <iostream>

#include "JelloWorld.h"
#include "Jello.h"
#include "PhysicsManager.h"

#define BOUND_X_MIN -2.0f
#define BOUND_Y_MIN -2.0f
#define BOUND_Z_MIN -2.0f
#define BOUND_X_MAX 2.0f
#define BOUND_Y_MAX 2.0f
#define BOUND_Z_MAX 2.0f

JelloWorld::JelloWorld(GameContext gameContext)
	: World(gameContext)
{
	m_jello = new Jello(m_gameContext, this, glm::vec3(0.0f, 0.0f, 0.0));
}

void JelloWorld::ToggleDrawingMode()
{
	m_jello->drawListElementValid = false;
	RenderMode = 1 - RenderMode;
	if (RenderMode == 1)
	{
		ShowStructuralSpring = 1;
		ShowShearSpring = 0;
		ShowBendSpring = 0;
	}
}

void JelloWorld::ToggleStructuralSpring()
{
	if (RenderMode == 1)
	{
		ShowStructuralSpring = 1 - ShowStructuralSpring;
	}
}

void JelloWorld::ToggleShearSpring()
{
	if (RenderMode == 1)
	{
		ShowShearSpring = 1 - ShowShearSpring;
	}
}

void JelloWorld::ToggleBendSpring()
{
	if (RenderMode == 1)
	{
		ShowBendSpring = 1 - ShowBendSpring;
	}
}

bool JelloWorld::LoadWorld(const char* fileName)
{
	int i, j, k;
	FILE * file;

	fopen_s(&file, fileName, "r");
	if (file == NULL) {
		printf("can't open file\n");
		return false;
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
		&m_jello->m_kElastic, &m_jello->m_dElastic, &m_kCollision, &m_dCollision);

	/* read mass of each of the 512 points */
	fscanf_s(file, "%f\n", &m_jello->m_mass);

	// Init jello cube, passing mass to particles
	m_jello->InitJello();

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

	m_jello->CreateSprings();

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
	return true;
}

void JelloWorld::Update(float Delta)
{
	m_jello->Update(Delta);
	CheckBoundary();
}

void JelloWorld::CheckBoundary()
{
	for (int i = 0; i < 8; i++)
	{
		for (int k = 0; k < 8; k++)
		{
			CheckBoundary(&m_jello->m_particles[i][0][k]);
			CheckBoundary(&m_jello->m_particles[i][7][k]);
		}
	}

	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			CheckBoundary(&m_jello->m_particles[i][j][0]);
			CheckBoundary(&m_jello->m_particles[i][j][7]);
		}
	}

	for (int k = 0; k < 8; k++)
	{
		for (int j = 0; j < 8; j++)
		{
			CheckBoundary(&m_jello->m_particles[0][j][k]);
			CheckBoundary(&m_jello->m_particles[7][j][k]);
		}
	}
}

void JelloWorld::CheckBoundary(class Particle* CurrentParticle)
{
	Particle* Anchor = new Particle();
	glm::vec3 OutwardDir = glm::vec3(0.0f);
	bool bOutOfBound = false;

	if (CurrentParticle->m_position.x < BOUND_X_MIN || CurrentParticle->m_position.x > BOUND_X_MAX)
	{
		bOutOfBound = true;
		Anchor->m_position.x = CurrentParticle->m_position.x < 0 ? BOUND_X_MIN : BOUND_X_MAX;
		OutwardDir.x = Anchor->m_position.x;
	}
	else
	{
		Anchor->m_position.x = CurrentParticle->m_position.x;
	}

	if (CurrentParticle->m_position.y < BOUND_Y_MIN || CurrentParticle->m_position.y > BOUND_Y_MAX)
	{
		bOutOfBound = true;
		Anchor->m_position.y = CurrentParticle->m_position.y < 0 ? BOUND_Y_MIN : BOUND_Y_MAX;
		OutwardDir.y = Anchor->m_position.y;
	}
	else
	{
		Anchor->m_position.y = CurrentParticle->m_position.y;
	}

	if (CurrentParticle->m_position.z < BOUND_Z_MIN || CurrentParticle->m_position.z > BOUND_Z_MAX)
	{
		bOutOfBound = true;
		Anchor->m_position.z = CurrentParticle->m_position.z < 0 ? BOUND_Z_MIN : BOUND_Z_MAX;
		OutwardDir.z = Anchor->m_position.z;
	}
	else
	{
		Anchor->m_position.z = CurrentParticle->m_position.z;
	}

	if (bOutOfBound)
	{
		m_gameContext.GetPhysicsManager()->GenerateCollisionInfo(CurrentParticle, Anchor, OutwardDir, m_kCollision, m_dCollision);
	}
	else
	{
		delete Anchor;
	}
}

glm::vec3 JelloWorld::GetForceInForceField(glm::vec3 position) const
{
	float X = position.x;
	float Y = position.y;
	float Z = position.z;

	if (X < BOUND_X_MIN || X > BOUND_X_MAX)
	{
		return glm::vec3(0.0f);
	}

	if (Y < BOUND_Y_MIN || Y > BOUND_Y_MAX)
	{
		return glm::vec3(0.0f);
	}

	if (Z < BOUND_Z_MIN || Z > BOUND_Z_MAX)
	{
		return glm::vec3(0.0f);
	}

	float resX = (X - BOUND_X_MIN) * (m_resolution - 1) / (BOUND_X_MAX - BOUND_X_MIN);
	float resY = (Y - BOUND_Y_MIN) * (m_resolution - 1) / (BOUND_Y_MAX - BOUND_Y_MIN);
	float resZ = (Z - BOUND_Z_MIN) * (m_resolution - 1) / (BOUND_Z_MAX - BOUND_Z_MIN);

	int xFloor		= (int)(resX);
	int yFloor		= (int)(resY);
	int zFloor		= (int)(resZ);
	int xCeiling	= (int)(resX +1);
	int yCeiling	= (int)(resY +1);
	int zCeiling	= (int)(resZ +1);

	int offsetX = m_resolution * m_resolution;
	int offsetY = m_resolution;
	glm::vec3 p1 = m_forceField[xFloor * offsetX + yFloor * offsetY + zFloor];
	glm::vec3 p2 = m_forceField[xCeiling * offsetX + yFloor * offsetY + zFloor];
	glm::vec3 p3 = m_forceField[xFloor * offsetX + yFloor * offsetY + zCeiling];
	glm::vec3 p4 = m_forceField[xCeiling * offsetX + yFloor * offsetY + zCeiling];
	glm::vec3 p5 = m_forceField[xFloor * offsetX + yCeiling * offsetY + zFloor];
	glm::vec3 p6 = m_forceField[xCeiling * offsetX + yCeiling * offsetY + zFloor];
	glm::vec3 p7 = m_forceField[xCeiling * offsetX + yCeiling * offsetY + zCeiling];
	glm::vec3 p8 = m_forceField[xFloor * offsetX + yCeiling * offsetY + zCeiling];

	// interpolating X
	float lowerX = (float)xCeiling - resX;
	float upperX = resX - (float)xFloor;
	glm::vec3 interp_x_1 = lowerX * p1 + upperX * p2;
	glm::vec3 interp_x_2 = lowerX * p3 + upperX * p4;
	glm::vec3 interp_x_3 = lowerX * p5 + upperX * p6;
	glm::vec3 interp_x_4 = lowerX * p7 + upperX * p8;

	// interpolating Y
	float lowerY = (float)yCeiling - resY;
	float upperY = resY - (float)yFloor;
	glm::vec3 interp_xy_1 = lowerY * interp_x_1 + upperY * interp_x_3;
	glm::vec3 interp_xy_2 = lowerY * interp_x_2 + upperY * interp_x_4;

	// interpolating Z
	glm::vec3 interp_xyz = ((float)zCeiling - resZ) * interp_xy_1 + (resZ - (float)zFloor) * interp_xy_2;

	return interp_xyz;
}
