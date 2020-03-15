#pragma once

#include "BEDirectXShader.h"

struct BEDXVertexShaderElement
{
	std::string name;
	DXGI_FORMAT format;
};

typedef std::vector<BEDXVertexShaderElement> BEDXVertexShaderLayout;



class BEDXVertexShader : public BEDirectXShader
{
public:

	BEDXVertexShader(BEDirectXDevice& device, std::string src, BEDXVertexShaderLayout layout);

	void Bind(BEDirectXDevice& device);

protected:
	BEDXVertexShader() = default;

	wrl::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	wrl::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;

	void CreateFromFile(BEDirectXDevice& device, std::string filename);
	void CreateFromSource(BEDirectXDevice& device, std::string source);
	void SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob, BEDXVertexShaderLayout& layout);
};


//////////////////////////////////////////////////////////////////////

class BEDXVertexShaderPosNorColTex : public BEDXVertexShader
{
public:
	BEDXVertexShaderPosNorColTex(BEDirectXDevice& device, std::string filename) {
		CreateFromFile(device, filename);
		SetLayout(device, *pBlob.Get(), layout);
	};

	static std::string UniqueId(std::string filename) { return GenerateUniqueId<BEDXVertexShaderPosNorColTex>(filename); };

protected:
	BEDXVertexShaderLayout layout = {
		{"Position", DXGI_FORMAT_R32G32B32A32_FLOAT},
		{"Normal", DXGI_FORMAT_R32G32B32A32_FLOAT},
		{"Color", DXGI_FORMAT_R32G32B32A32_FLOAT},
		{"Texcoord", DXGI_FORMAT_R32G32_FLOAT}
	};
};


//////////////////////////////////////////////////////////////////////

class BEDXVertexShaderPosTex : public BEDXVertexShader
{
public:
	BEDXVertexShaderPosTex(BEDirectXDevice& device, std::string filename) {
		CreateFromFile(device, filename);
		SetLayout(device, *pBlob.Get(), layout);
	};

	static std::string UniqueId(std::string filename) { return GenerateUniqueId<BEDXVertexShaderPosTex>(filename); };

protected:
	BEDXVertexShaderLayout layout = {
		{"Position", DXGI_FORMAT_R32G32B32A32_FLOAT},
		{"Texcoord", DXGI_FORMAT_R32G32_FLOAT}
	};
};