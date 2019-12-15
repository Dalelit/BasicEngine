#pragma once
#include "BEDXTexture.h"
class BEDXTextureUpdateable : public BEDXTexture
{
public:
	BEDXTextureUpdateable(BEDirectXDevice& device, unsigned int width, unsigned int height, unsigned int pitch, void* source, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_SNORM);
	~BEDXTextureUpdateable() = default;

	void UpdateFromSource(BEDirectXDevice& device);

private:
	void* sourceBuffer = nullptr;
	unsigned int sourcePitch = 0u;
};

