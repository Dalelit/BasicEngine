#pragma once

#include "BEDirectXResource.h"

class BEDXVertexShader : public BEDirectXResource
{
public:
	BEDXVertexShader(BEDirectXDevice& device, LPCWSTR filename);

	void Bind(BEDirectXDevice& device);

private:
	wrl::ComPtr<ID3D11VertexShader> pVertexShader = NULL;
	wrl::ComPtr<ID3D11InputLayout> pInputLayout = NULL;
};

