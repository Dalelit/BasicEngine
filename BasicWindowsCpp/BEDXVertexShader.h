#pragma once

#include "BEDirectXResource.h"

class BEDXVertexShader : public BEDirectXResource
{
public:

	BEDXVertexShader(BEDirectXDevice& device, std::string filename);

	void Bind(BEDirectXDevice& device);

protected:
	wrl::ComPtr<ID3D11VertexShader> pVertexShader = NULL;
	wrl::ComPtr<ID3D11InputLayout> pInputLayout = NULL;
	std::string filename;

	void Initialise(BEDirectXDevice& device);
	virtual void SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob) = 0;
};

//////////////////////////////////////////////////////////////////////

class BEDXVertexShaderPosNorColTex : public BEDXVertexShader
{
public:
	BEDXVertexShaderPosNorColTex(BEDirectXDevice& device, std::string filename) : BEDXVertexShader(device, filename) { Initialise(device); };

	static std::string UniqueId(std::string filename) { return GenerateUniqueId<BEDXVertexShaderPosNorColTex>(filename); };

protected:
	void SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob);
};

//////////////////////////////////////////////////////////////////////

class BEDXVertexShaderPosTex : public BEDXVertexShader
{
public:
	BEDXVertexShaderPosTex(BEDirectXDevice& device, std::string filename) : BEDXVertexShader(device, filename) { Initialise(device); };

	static std::string UniqueId(std::string filename) { return GenerateUniqueId<BEDXVertexShaderPosTex>(filename); };

protected:
	void SetLayout(BEDirectXDevice& device, ID3DBlob& pBlob);
};