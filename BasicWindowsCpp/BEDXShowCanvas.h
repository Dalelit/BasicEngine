#pragma once

#include "BEDirectX.h"
#include "BEDXTextureUpdateable.h"
#include "BEDXVertexBuffer.h"
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"


class BEDXShowCanvas
{
public:
	BEDXShowCanvas(BEDirectXDevice& device, BESurface2D<DirectX::XMVECTOR>& surface, bool updateable = false);

	void Draw(BEDirectXDevice& device);

private:
	// To do: updateable bool and 2 pointers is a hack for now
	std::unique_ptr<BEDXTexture> pTex;
	std::unique_ptr<BEDXTextureUpdateable> pTexUpdt;

	std::unique_ptr<BEDXVertexBuffer> vertexBuffer;
	std::unique_ptr<BEDXVertexShader> vertexShader;
	std::unique_ptr<BEDXPixelShader>  pixelShader;
};

