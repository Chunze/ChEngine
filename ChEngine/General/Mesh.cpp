#include "Mesh.h"

Mesh::Mesh(GameContext* gameContext)
	: PrimitiveComponent(gameContext)
{

}

Mesh::Mesh(GameContext* gameContext, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: PrimitiveComponent(gameContext)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;

}

void Mesh::AddDrawListElement()
{
	m_DrawListElement.vertexBuffer = (float*)&m_Vertices[0];
	m_DrawListElement.VBsize_inByte = sizeof(m_Vertices) * m_Vertices.size();
	m_DrawListElement.attributeSizes.push_back(3);
	m_DrawListElement.attributeSizes.push_back(3);
	m_DrawListElement.attributeSizes.push_back(2);
	m_DrawListElement.vertextInfoSize = 8;

	m_gameContext->GetDrawList()->AddToDrawQ(m_DrawListElement);
}
