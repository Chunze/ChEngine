#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture(const char* TexturePath, bool bFlipVertical = false);

	void Bind();

	unsigned char* LoadImage(const char* TexturePath, bool bFlipVertical);

	int width, height, nrChannels;

	unsigned int textureID;
	std::string m_Type;

	std::string GetFilePath() { return m_FilePath; }

protected:
	std::string m_FilePath;
	
};

#endif