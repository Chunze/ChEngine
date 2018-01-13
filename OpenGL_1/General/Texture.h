#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:
	Texture(const char* TexturePath, bool bHasTransparency = false, bool bFlipVertical = false);

	void Bind();

	unsigned char* LoadImage(const char* TexturePath, bool bFlipVertical);

	int width, height, nrChannels;

	unsigned int textureID;
};

#endif