#pragma once
#include "glm.h"
#include "DrawList.h"
#include "PrimitiveComponent.h"
#include "Texture.h"

#include <string>
#include <vector>

class Shader;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

// struct Texture {
// 	unsigned int id;
// 	std::string type;
// 	std::string path;
// };

class Mesh : public PrimitiveComponent
{
public:
	Mesh(GameContext* gameContext);

	Mesh(GameContext* gameContext, 
		std::vector<Vertex> vertices, 
		std::vector<unsigned int> indices, 
		std::vector<Texture> textures);

	/*  Mesh Data  */
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

	void AddDrawListElement() override;

};