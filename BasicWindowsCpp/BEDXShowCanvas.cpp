#include "pch.h"
#include "BEDXShowCanvas.h"

using namespace DirectX;

BEDXShowCanvas::BEDXShowCanvas(BEDirectXDevice& device, BESurface2D<XMVECTOR>& surface, bool updateable)
{
	if (updateable)
		pTexUpdt = std::make_unique<BEDXTextureUpdateable>(device, surface, DXGI_FORMAT_R32G32B32A32_FLOAT);
	else
		pTex = std::make_unique<BEDXTexture>(device, surface, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// set the verticies
	verticies[0] = { { -1,-1,0 }, {0,0} };
	verticies[1] = { { -1, 1,0 }, {0,1} };
	verticies[2] = { {  1, 1,0 }, {1,1} };

	verticies[3] = { { -1,-1,0 }, {0,0} };
	verticies[4] = { {  1, 1,0 }, {1,1} };
	verticies[5] = { {  1,-1,0 }, {1,0} };

	// create the vertex buffer
	vertexBuffer = std::make_unique<BEDXVertexBuffer>(device, &verticies, (unsigned int)ARRAYSIZE(verticies), (unsigned int)sizeof(BETexVertex));
	vertexBuffer->Bind(device);

	// create the vertex shader
	vertexShader = std::make_unique<BEDXVertexShaderPosTex>(device, L"VSPassthrough.cso");
	vertexShader->Bind(device);

	// create the pixel shader
	pixelShader = std::make_unique<BEDXPixelShader>(device, L"PSPassthrough.cso");
	pixelShader->Bind(device);
}

void BEDXShowCanvas::Draw(BEDirectXDevice& device)
{
	if (pTexUpdt)
	{
		pTexUpdt->UpdateFromSource(device);
		pTexUpdt->Bind(device);
	}
	else
	{
		pTex->Bind(device);
	}

	vertexBuffer->Draw(device);
}
