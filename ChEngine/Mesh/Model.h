#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PrimitiveComponent.h"

class Mesh;
struct Texture;

class Model : public PrimitiveComponent
{
public:
	Model(GameContext* gameContext, char *path);
	~Model();

	void AddDrawListElement() override;

protected:
	std::vector<Mesh> m_Meshes;
	std::vector<Texture> m_TextureLoaded;
	std::string m_Directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
};

