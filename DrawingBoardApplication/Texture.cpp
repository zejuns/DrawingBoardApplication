#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(const char* InPath, bool InGammaCorrection)
{
	LoadTexture(InPath, textureId, width, height, channel, InGammaCorrection);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

bool Texture::LoadTexture(const char* InPath, unsigned int& OutId, int& OutW, int& OutH, int& OutChannel, bool InGammaCorrection /*= false*/)
{
	bool result = false;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(InPath, &OutW, &OutH, &OutChannel, 0);
	if (data)
	{
		std::cout << "Success to load texture: " << InPath << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture: " << InPath << std::endl;
		result = false;
		return result;
	}
	glGenTextures(1, &OutId);
	glBindTexture(GL_TEXTURE_2D, OutId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (InGammaCorrection)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	GLenum format = GL_RED;
	if (OutChannel == 1)format = GL_RED;
	else if (OutChannel == 3)format = GL_RGB;
	else if (OutChannel == 4)format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, OutW, OutH, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	std::cout << "Load texture: " << InPath << std::endl;
	std::cout << "\t" << "Width: " << OutW << " Height: " << OutH << " Channel: " << OutChannel << std::endl;
	result = true;

	stbi_image_free(data);

	return  result;
}

void Texture::SetTexture(unsigned char* InPixels, int InW, int InH)
{
	width = InW;
	height = InH;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto format = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, InPixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(InPixels);
}

void Texture::Bind(unsigned int InSlot) const
{
	glActiveTexture(GL_TEXTURE0 + InSlot);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}