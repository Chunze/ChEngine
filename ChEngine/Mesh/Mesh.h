#pragma once
#include "glm.h"
#include "DrawList.h"
#include "BaseClass.h"
#include "Texture.h"

#include <string>
#include <vector>


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh : public BaseClass
{
	typedef BaseClass Super;
public:
	Mesh(GameContext* gameContext,
		std::vector<Vertex> vertices, 
		std::vector<unsigned int> indices, 
		std::vector<Texture> textures);

	/*  Mesh Data  */
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

	DrawListElement& GetDrawListElement();

private:
	DrawListElement m_DrawListElement;

	// Get DrawListElement ready to pickup
	void InitRenderInfo();

};