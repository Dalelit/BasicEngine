#pragma once

#include "BEDirectXResource.h"

class BEDXVertexShader : public BEDirectXResource
{
public:

	enum class InputLayout {
		POSITION4_NORMAL4_COLOR4,
		POSITION3_TEXCOORD2
	};

	BEDXVertexShader(BEDirectXDevice& device, LPCWSTR filename, InputLayout layout);

	void Bind(BEDirectXDevice& device);

private:
	wrl::ComPtr<ID3D11VertexShader> pVertexShader = NULL;
	wrl::ComPtr<ID3D11InputLayout> pInputLayout = NULL;

	void SetInputLayoutPos4Nor4Col4(BEDirectXDevice& device, ID3DBlob& shaderBlob);
	void SetInputLayoutPos3Tex2(BEDirectXDevice& device, ID3DBlob& shaderBlob);

};

