#include <iostream>
#include "Jello.h"
#include "JelloWorld.h"
#include "ForceGenerators/BasicSprintFG.h"
#include "PhysicsManager.h"

#define point glm::vec3
#define pDIFFERENCE(src1,src2,dest) dest = src1 - src2;
#define CROSSPRODUCTp(vector1,vector2,dest) dest = glm::cross(vector1, vector2);
#define MULTIPLY(src1,src2,dest) dest = src1 * src2;
#define pNORMALIZE(vector) vector = glm::normalize(vector);
#define pSUM(src1, src2, dest) dest = src1 + src2;


#define NODE(face,i,j) ((Particle* )(m_particles) + pointMap((face),(i),(j)))->m_position

#define PROCESS_NEIGHBOUR(di, dj, dk, r, g, b) \
	ip=i+(di);\
	jp=j+(dj);\
	kp=k+(dk);\
	if\
	(!( (ip>7) || (ip<0) || (jp>7) || (jp<0) || (kp>7) || (kp<0) ) \
		&& ((i==0) || (i==7) || (j==0) || (j==7) || (k==0) || (k==7))\
		&& ((ip==0) || (ip==7) || (jp==0) || (jp==7) || (kp==0) || (kp==7))) \
	{\
		SpringRenderInfo.push_back(m_particles[i][j][k].m_position.x); \
		SpringRenderInfo.push_back(m_particles[i][j][k].m_position.y); \
		SpringRenderInfo.push_back(m_particles[i][j][k].m_position.z); \
		SpringRenderInfo.push_back(r); \
		SpringRenderInfo.push_back(g); \
		SpringRenderInfo.push_back(b); \
		SpringRenderInfo.push_back(m_particles[ip][jp][kp].m_position.x); \
		SpringRenderInfo.push_back(m_particles[ip][jp][kp].m_position.y); \
		SpringRenderInfo.push_back(m_particles[ip][jp][kp].m_position.z); \
		SpringRenderInfo.push_back(r); \
		SpringRenderInfo.push_back(g); \
		SpringRenderInfo.push_back(b); \
	}\

Jello::Jello(GameContext* gameContext, World* world, glm::vec3 position, float size)
	: GameObject(gameContext, world),
	  m_position(position),
	  m_size(size)
{
	// init shader
	shader = new Shader("JelloVS.vert", "JelloFS.frag");
}

Jello::~Jello()
{
	
}

void Jello::InitJello()
{
	m_step = m_size / 7.0f;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				glm::vec3 particlePos = glm::vec3(i * m_step, j * m_step, k * m_step);
				Particle* temp = new Particle(m_mass, particlePos, glm::vec3(0.0));
				m_particles[i][j][k] = *temp;
			}
		}
	}
}

void Jello::CreateAndAddDrawListElement(int Mode)
{
	if (Mode == 0) // Triangle mode
	{
		if (!e.shader.IsValid())
		{
			e.shader = *(new Shader("JelloVS.vert", "JelloFS.frag"));
		}

		UpdateVertex();
		e.vertexBuffer = Vertices;
		e.VBsize_inByte = 4032 * sizeof(float);
		e.drawingPrimitive = DrawingPrimitives::TRIANGLE_STRIP;

		e.attributeSizes.push_back(3);
		e.attributeSizes.push_back(3);
		e.vertextInfoSize = 6;
		m_gameContext->GetDrawList()->AddToDrawQ(e);
	}
	else // wire frame mode
	{
		if (!ParticleRender.shader.IsValid())
		{
			ParticleRender.shader = *(new Shader("DebugDrawShader.vert", "DebugDrawShader.frag"));
			SpringRender.shader = ParticleRender.shader;
		}
		
		UpdateParticleRenderInfo();
		ParticleRender.vertexBuffer = ParticleRenderInfo;
		ParticleRender.VBsize_inByte = 3072 * sizeof(float);
		ParticleRender.drawingPrimitive = DrawingPrimitives::POINTS;
		ParticleRender.attributeSizes.push_back(3);
		ParticleRender.attributeSizes.push_back(3);
		ParticleRender.vertextInfoSize = 6;
		m_gameContext->GetDrawList()->AddToDrawQ(ParticleRender);

		UpdateSpringRenderInfo();
		if (SpringRenderInfo.size() != 0)
		{
			SpringRender.vertexBuffer = &SpringRenderInfo[0];
			SpringRender.VBsize_inByte = SpringRenderInfo.size() * sizeof(float);
			SpringRender.drawingPrimitive = DrawingPrimitives::LINES;
			SpringRender.attributeSizes.push_back(3);
			SpringRender.attributeSizes.push_back(3);
			SpringRender.vertextInfoSize = 6;
			m_gameContext->GetDrawList()->AddToDrawQ(SpringRender);
		}
	}
}

void Jello::Update(float Delta)
{
	int Mode = m_gameContext->GetPhysicsManager()->GetIntegrator();

	// add to drawlist
	if (Mode == 0 || (Mode == 1 && m_gameContext->GetPhysicsManager()->GetRK4StepCount() == 4))
	{
		int RenderMode = static_cast<JelloWorld*>(GetWorld())->RenderMode;
		CreateAndAddDrawListElement(RenderMode);
	}

	if (Delta == 0.0f) { return; }

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				// COMMENT: adding external force
				glm::vec3 externalForce = ((JelloWorld*)GetWorld())->GetForceInForceField(m_particles[i][j][k].m_position);
				m_particles[i][j][k].addForce(externalForce);
			}
		}
	}

	if (Mode == 0 /* Euler */)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				for (int k = 0; k < 8; k++)
				{
					m_particles[i][j][k].Integrate_Euler(Delta);
				}
			}
		}
	}
	else if (Mode == 1 /* RK4 */)
	{
		int step = m_gameContext->GetPhysicsManager()->GetRK4StepCount();
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				for (int k = 0; k < 8; k++)
				{
					m_particles[i][j][k].Integrate_Rk4(Delta, step);
				}
			}
		}
	}
	
	
}

void Jello::UpdateVertex()
{
	auto pointMap = [](int side, int i, int j)->int
	{
		int r;

		switch (side)
		{
		case 1: //[i][j][0] bottom face
			r = 64 * i + 8 * j;
			break;
		case 6: //[i][j][7] top face
			r = 64 * i + 8 * j + 7;
			break;
		case 2: //[i][0][j] front face
			r = 64 * i + j;
			break;
		case 5: //[i][7][j] back face
			r = 64 * i + 56 + j;
			break;
		case 3: //[0][i][j] left face
			r = 8 * i + j;
			break;
		case 4: //[7][i][j] right face
			r = 448 + 8 * i + j;
			break;
		}

		return r;
	};

	int i, j;
	point r1, r2, r3;
	/* normals buffer and counter for Gourad shading*/
	point normal[8][8];
	int counter[8][8];

	int face;
	float faceFactor;

	if (fabs(m_particles[0][0][0].m_position.x) > 30)
	{
		printf("Your cube somehow escaped way out of the box.\n");
		//exit(0);
	}

	int Index = 0;
	// face == face of a cube
	// 1 = bottom, 2 = front, 3 = left, 4 = right, 5 = far, 6 = top
	for (face = 1; face <= 6; face++)
	{

		if ((face == 1) || (face == 3) || (face == 5))
		{
			// flip orientation
			faceFactor = -1;
		}
		else
		{
			faceFactor = 1;
		}


		for (i = 0; i <= 7; i++) // reset buffers
		{
			for (j = 0; j <= 7; j++)
			{
				normal[i][j].x = 0; normal[i][j].y = 0; normal[i][j].z = 0;
				counter[i][j] = 0;
			}
		}

		/* process triangles, accumulate normals for Gourad shading */

		for (i = 0; i <= 6; i++)
		{
			for (j = 0; j <= 6; j++) // process block (i,j)
			{
				r1 = NODE(face, i + 1, j) - NODE(face, i, j); // first triangle
				r2 = NODE(face, i, j + 1) - NODE(face, i, j);
				r3 = glm::cross(r1, r2);
				r3 *= faceFactor;
				r3 = glm::normalize(r3);
				normal[i + 1][j] = normal[i + 1][j] + r3;
				counter[i + 1][j]++;
				normal[i][j + 1] = normal[i][j + 1] + r3;
				counter[i][j + 1]++;
				normal[i][j] = normal[i][j] + r3;
				counter[i][j]++;

				pDIFFERENCE(NODE(face, i, j + 1), NODE(face, i + 1, j + 1), r1); // second triangle
				pDIFFERENCE(NODE(face, i + 1, j), NODE(face, i + 1, j + 1), r2);
				CROSSPRODUCTp(r1, r2, r3); MULTIPLY(r3, faceFactor, r3);
				pNORMALIZE(r3);
				pSUM(normal[i + 1][j], r3, normal[i + 1][j]);
				counter[i + 1][j]++;
				pSUM(normal[i][j + 1], r3, normal[i][j + 1]);
				counter[i][j + 1]++;
				pSUM(normal[i + 1][j + 1], r3, normal[i + 1][j + 1]);
				counter[i + 1][j + 1]++;
			}
		}

		/* the actual rendering */
		for (j = 1; j <= 7; j++)
		{
			// TODO: add this for the core profile

			// 			if (faceFactor > 0)
			// 				glFrontFace(GL_CCW); // the usual definition of front face
			// 			else
			// 				glFrontFace(GL_CW); // flip definition of orientation

			for (i = 0; i <= 7; i++)
			{
				/* shader input layout:
				layout(location = 0) in vec3 position;
				layout(location = 0) in vec3 normal;
				*/
				// vertex 1
				int index = pointMap(face, i, j);

				glm::vec3 v1 = ((Particle*)(m_particles)+index)->m_position;
				Vertices[Index] = v1.x;
				Index++;
				Vertices[Index] = v1.y;
				Index++;
				Vertices[Index] = v1.z;
				Index++;

				// normal 1
				Vertices[Index] = normal[i][j].x / counter[i][j];
				Index++;
				Vertices[Index] = normal[i][j].y / counter[i][j];
				Index++;
				Vertices[Index] = normal[i][j].z / counter[i][j];
				Index++;

				// vertex 2
				index = pointMap(face, i, j - 1);
				glm::vec3 v2 = ((Particle*)(m_particles)+index)->m_position;
				Vertices[Index] = v2.x;
				Index++;
				Vertices[Index] = v2.y;
				Index++;
				Vertices[Index] = v2.z;
				Index++;

				// normal 2
				Vertices[Index] = normal[i][j - 1].x / counter[i][j - 1];
				Index++;
				Vertices[Index] = normal[i][j - 1].y / counter[i][j - 1];
				Index++;
				Vertices[Index] = normal[i][j - 1].z / counter[i][j - 1];
				Index++;

			}
		}
	}
}

void Jello::UpdateParticleRenderInfo()
{
	int index = 0;
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			for (int k = 0; k <= 7; k++)
			{
				if (i * j * k * (7 - i) * (7 - j) * (7 - k) != 0) { continue; }
				// position
				ParticleRenderInfo[index++] = m_particles[i][j][k].m_position.x;
				ParticleRenderInfo[index++] = m_particles[i][j][k].m_position.y;
				ParticleRenderInfo[index++] = m_particles[i][j][k].m_position.z;
				// color
				ParticleRenderInfo[index++] = 0.0f;
				ParticleRenderInfo[index++] = 0.0f;
				ParticleRenderInfo[index++] = 0.0f;
			}
		}
	}
}

void Jello::UpdateSpringRenderInfo()
{
	SpringRenderInfo.clear();

	int ip, jp, kp;

	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			for (int k = 0; k <= 7; k++)
			{
				if (i * j * k * (7 - i) * (7 - j) * (7 - k) != 0) { continue; }

				JelloWorld* _jelloWorld = static_cast<JelloWorld*>(GetWorld());

				if (_jelloWorld->ShowStructuralSpring == 1)
				{
					float r = 0.0f;
					float g = 0.0f;
					float b = 1.0f;
					PROCESS_NEIGHBOUR(1, 0, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, 1, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, 0, 1, r, g, b);
					PROCESS_NEIGHBOUR(-1, 0, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, -1, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, 0, -1, r, g, b);
				}

				if (_jelloWorld->ShowShearSpring == 1)
				{
					float r = 0.0f;
					float g = 1.0f;
					float b = 0.0f;
					PROCESS_NEIGHBOUR(1, 1, 0, r, g, b);
					PROCESS_NEIGHBOUR(-1, 1, 0, r, g, b);
					PROCESS_NEIGHBOUR(-1, -1, 0, r, g, b);
					PROCESS_NEIGHBOUR(1, -1, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, 1, 1, r, g, b);
					PROCESS_NEIGHBOUR(0, -1, 1, r, g, b);
					PROCESS_NEIGHBOUR(0, -1, -1, r, g, b);
					PROCESS_NEIGHBOUR(0, 1, -1, r, g, b);
					PROCESS_NEIGHBOUR(1, 0, 1, r, g, b);
					PROCESS_NEIGHBOUR(-1, 0, 1, r, g, b);
					PROCESS_NEIGHBOUR(-1, 0, -1, r, g, b);
					PROCESS_NEIGHBOUR(1, 0, -1, r, g, b);
				}

				if (_jelloWorld->ShowBendSpring == 1)
				{
					float r = 1.0f;
					float g = 0.0f;
					float b = 0.0f;
					PROCESS_NEIGHBOUR(2, 0, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, 2, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, 0, 2, r, g, b);
					PROCESS_NEIGHBOUR(-2, 0, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, -2, 0, r, g, b);
					PROCESS_NEIGHBOUR(0, 0, -2, r, g, b);
				}
			}
		}
	}
}

void Jello::CreateSprings()
{
	InitJello();

	float sqrt2 = 1.41421356237f;
	float segment = m_step;
	int i, j, k, offset;
	// Create spring force generators
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			for (k = 0; k <= 7; k++)
			{
				for (offset = -2; offset <= 2; offset++)
				{
					if (offset == 0)
					{
						continue;
					}

					if ((i + offset >= 0 && i + offset <= 7))
					{
						BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i + offset][j][k], segment * abs(offset), m_kElastic, m_dElastic);
						m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
					}

					if ((j + offset >= 0 && j + offset <= 7))
					{
						BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i][j + offset][k], segment * abs(offset), m_kElastic, m_dElastic);
						m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
					}

					if ((k + offset >= 0 && k + offset <= 7))
					{
						BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i][j][k + offset], segment * abs(offset), m_kElastic, m_dElastic);
						m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
					}

					if (offset == -2 || offset == 2)
					{
						continue;
					}

					if (i + offset >= 0 && i + offset <= 7)
					{
						if (j + offset >= 0 && j + offset <= 7)
						{
							BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i + offset][j + offset][k], segment * sqrt2, m_kElastic, m_dElastic);
							m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
						}

						if (j - offset >= 0 && j - offset <= 7)
						{
							BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i + offset][j - offset][k], segment * sqrt2, m_kElastic, m_dElastic);
							m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
						}

						if (k + offset >= 0 && k + offset <= 7)
						{
							BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i + offset][j][k + offset], segment * sqrt2, m_kElastic, m_dElastic);
							m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
						}

						if (k - offset >= 0 && k - offset <= 7)
						{
							BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i + offset][j][k - offset], segment * sqrt2, m_kElastic, m_dElastic);
							m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
						}
					}

					if (j + offset >= 0 && j + offset <= 7)
					{
						if (k + offset >= 0 && k + offset <= 7)
						{
							BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i][j + offset][k + offset], segment * sqrt2, m_kElastic, m_dElastic);
							m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
						}

						if (k - offset >= 0 && k - offset <= 7)
						{
							BasicSprintFG *sprintFG = new BasicSprintFG(&m_particles[i][j + offset][k - offset], segment * sqrt2, m_kElastic, m_dElastic);
							m_gameContext->GetPhysicsManager()->registerForce(&m_particles[i][j][k], sprintFG);
						}
					}

					
				}

			}
		}
	}
}

