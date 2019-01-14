#include <iostream>
#include <../stb_image.h>
#include <glad/glad.h>

#include "Texture.h"



Texture::Texture(const char* TexturePath, bool bFlipVertical)
{
	m_FilePath = std::string(TexturePath);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// reading the texture file
	unsigned char *data = LoadImage(TexturePath, bFlipVertical);

	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
		{
			format = GL_RED;
		}
		else if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// cleaning the memory
	stbi_image_free(data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

unsigned char* Texture::LoadImage(const char* TexturePath, bool bFlipVertical)
{
	if (bFlipVertical)
	{
		stbi_set_flip_vertically_on_load(true);
	}

	return stbi_load(TexturePath, &width, &height, &nrChannels, 0);
}
