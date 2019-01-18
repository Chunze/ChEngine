#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "BaseClass.h"
#include "DrawList.h"

class Mesh;
class Texture;

class Model : public BaseClass
{
	typedef BaseClass Super;
public:
	Model(GameContext* gameContext, char *path);
	~Model();

	std::vector<DrawListElement>& GetDrawListElement();

protected:
	std::vector<Mesh> m_Meshes;
	std::vector<Texture> m_TextureLoaded;
	std::vector<DrawListElement> m_DrawListElements;

	std::string m_Directory;
	Shader* m_Shader;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, 
											  aiTextureType type,
											  std::string typeName);

private:
	bool bDirty = true;
};

