#pragma once
#include "BEDirectXResource.h"
class BEDXTexture : public BEDirectXResource
{
public:
	BEDXTexture(BEDirectXDevice& device, unsigned int width, unsigned int height, unsigned int pitch, void* source, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_SNORM);
	~BEDXTexture() = default;

	void Bind(BEDirectXDevice& device);

	unsigned int slot = 0u;

protected:
	BEDXTexture() = default; // to do: is this a good way?

	Microsoft::WRL::ComPtr<ID3D11Texture2D1> pTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState = nullptr;
};

