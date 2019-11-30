#pragma once

#include "BEDirectXResource.h"

class BEDXPixelShader : public BEDirectXResource
{
public:

	BEDXPixelShader(BEDirectXDevice& device, LPCWSTR filename);

	void Bind(BEDirectXDevice& device);

private:
	wrl::ComPtr<ID3D11PixelShader> pPixelShader = NULL;

};

