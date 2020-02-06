#pragma once

#include "BEDirectXResource.h"

class BEDXPixelShader : public BEDirectXResource
{
public:

	BEDXPixelShader(BEDirectXDevice& device, std::wstring filename);

	void Bind(BEDirectXDevice& device);

	static std::wstring UniqueId(std::wstring filename) { return GenerateUniqueId<BEDXPixelShader>(filename); };

private:
	wrl::ComPtr<ID3D11PixelShader> pPixelShader = NULL;
	std::wstring filename;
};

