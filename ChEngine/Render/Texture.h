#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture(const std::string FileName, const std::string Directory);

	Texture(const char* TexturePath, bool bFlipVertical = false);

	void Bind();

	unsigned char* LoadImage(const char* TexturePath, bool bFlipVertical = false);

	int width, height, nrChannels;

	unsigned int textureID;
	std::string m_Type;

	std::string GetFilePath() { return m_FilePath; }

	std::string GetFileName() { return m_FileName; }

protected:
	std::string m_FilePath;
	std::string m_FileName;

	void Init(std::string FilePath);
};

#endif