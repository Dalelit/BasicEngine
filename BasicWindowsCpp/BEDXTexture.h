#pragma once
#include "BEDirectXResource.h"
#include "BETexture.h"

class BEDXTexture : public BEDirectXResource
{
public:
	BEDXTexture(BEDirectXDevice& device, BESurface2D<DirectX::XMVECTOR>& surface, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_SNORM);
	BEDXTexture(BEDirectXDevice& device, BETexture& texture, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);
	~BEDXTexture() = default;

	void Bind(BEDirectXDevice& device);

	static std::wstring UniqueId(std::wstring filename) { return GenerateUniqueId<BEDXTexture>(filename); };

	unsigned int slot = 0u;

protected:
	BEDXTexture() = default; // to do: is this a good way?

	Microsoft::WRL::ComPtr<ID3D11Texture2D1> pTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState = nullptr;
};

