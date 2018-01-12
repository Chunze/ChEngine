#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:
	Texture(const char* TexturePath);

	int width, height, nrChannels;

	unsigned int textureID;
};

#endif