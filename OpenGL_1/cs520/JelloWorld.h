#pragma once
#include "World.h"
#include "glm.h"

class JelloWorld : public World
{
public:
	JelloWorld();

	class Jello* m_jello;

	char m_intergrator[10];		// "RK4" or "Euler"
	float m_dt;				// timestep, e.g.. 0.001
	int m_n;					// display only every nth timestep
	int m_incPlanePresent;		// Is the inclined plane present? 1 = YES, 0 = NO
	float a, b, c, d;			// inclined plane has equation a * x + b * y + c * z + d = 0;
	int m_resolution;			// resolution for the 3d grid specifying the external force field; value of 0 means that there is no force field
	glm::vec3 * m_forceField;	// pointer to the array of values of the force field

	int viewingMode;

	virtual void LoadWorld(const char* fileName);
	virtual void Update(float Delta);
};