#pragma once
#include "BEDXTexture.h"
class BEDXTextureUpdateable : public BEDXTexture
{
public:
	BEDXTextureUpdateable(BEDirectXDevice& device, BESurface2D<DirectX::XMVECTOR>& surface, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_SNORM);
	~BEDXTextureUpdateable() = default;

	void UpdateFromSource(BEDirectXDevice& device);

private:
	BESurface2D<DirectX::XMVECTOR>& surface;
};

