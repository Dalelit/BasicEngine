#pragma once

#include "BEDirectXResource.h"

class BEDXVertexShader : public BEDirectXResource
{
public:

	BEDXVertexShader(BEDirectXDevice& device, std::wstring filename);

	void Bind(BEDirectXDevice& device);

protected:
	wrl::ComPtr<ID3D11VertexShader> pVertexShader = NULL;
	wrl::ComPtr<ID3D11InputLayout> pInputLayout = NULL;
	std::wstring filename;

	void Initialise(BEDirectXDevice& device);
	virtual void SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob) = 0;
};

//////////////////////////////////////////////////////////////////////

class BEDXVertexShaderPosNorColTex : public BEDXVertexShader
{
public:
	BEDXVertexShaderPosNorColTex(BEDirectXDevice& device, std::wstring filename) : BEDXVertexShader(device, filename) { Initialise(device); };

	static std::wstring UniqueId(std::wstring filename) { return GenerateUniqueId<BEDXVertexShaderPosNorColTex>(filename); };

protected:
	void SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob);
};

//////////////////////////////////////////////////////////////////////

class BEDXVertexShaderPosTex : public BEDXVertexShader
{
public:
	BEDXVertexShaderPosTex(BEDirectXDevice& device, std::wstring filename) : BEDXVertexShader(device, filename) { Initialise(device); };

	static std::wstring UniqueId(std::wstring filename) { return GenerateUniqueId<BEDXVertexShaderPosTex>(filename); };

protected:
	void SetLayout(BEDirectXDevice& device, ID3DBlob& pBlob);
};