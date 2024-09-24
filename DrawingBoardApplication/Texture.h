#pragma once

#include "Core.h"

class Texture
{
public:
	unsigned int textureId;
	int width;
	int height;
	int channel;

public:
	Texture();
	Texture(const char* InPath, bool InGammaCorrection = false);
	
	virtual ~Texture();
	static bool LoadTexture(const char* InPath, unsigned int& OutId, int& OutW, int& OutH, int& OutChannel, bool InGammaCorrection = false);

	void SetTexture(unsigned char* InPixels,int InW,int InH);


	virtual void Bind(unsigned int InSlot = 0) const;
	virtual void UnBind() const;
};

