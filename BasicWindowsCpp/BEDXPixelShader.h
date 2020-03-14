#pragma once

#include "BEDirectXResource.h"

class BEDXPixelShader : public BEDirectXResource
{
public:

	BEDXPixelShader(BEDirectXDevice& device, std::string filename);

	void Bind(BEDirectXDevice& device);

	static std::string UniqueId(std::string filename) { return GenerateUniqueId<BEDXPixelShader>(filename); };

private:
	wrl::ComPtr<ID3D11PixelShader> pPixelShader = NULL;
	std::string filename;
};

