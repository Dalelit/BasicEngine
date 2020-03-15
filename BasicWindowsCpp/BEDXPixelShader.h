#pragma once

#include "BEDirectXShader.h"

class BEDXPixelShader : public BEDirectXShader
{
public:
	void Bind(BEDirectXDevice& device);

protected:
	BEDXPixelShader() = default;
	wrl::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
};



class BEDXPixelShaderFile : public BEDXPixelShader
{
public:

	BEDXPixelShaderFile(BEDirectXDevice& device, std::string filename);

	static std::string UniqueId(std::string filename) { return GenerateUniqueId<BEDXPixelShaderFile>(filename); };

protected:
	std::string filename;
};



class BEDXPixelShaderSource : public BEDXPixelShader
{
public:

	BEDXPixelShaderSource(BEDirectXDevice& device, std::string source);
};