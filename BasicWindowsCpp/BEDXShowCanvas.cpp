#include "BEDXShowCanvas.h"
#include "BEDXVertexBuffer.h"
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"

using namespace DirectX;

BEDXShowCanvas::BEDXShowCanvas(BEDirectXDevice& device, BESurface2D<XMVECTOR>& surface, bool updateable)
{
	if (updateable)
		pTexUpdt = new BEDXTextureUpdateable(device, surface, DXGI_FORMAT_R32G32B32A32_FLOAT);
	else
		pTex = new BEDXTexture(device, surface, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// set the verticies
	verticies[0] = { { -1,-1,0 }, {0,0} };
	verticies[1] = { { -1, 1,0 }, {0,1} };
	verticies[2] = { {  1, 1,0 }, {1,1} };

	verticies[3] = { { -1,-1,0 }, {0,0} };
	verticies[4] = { {  1, 1,0 }, {1,1} };
	verticies[5] = { {  1,-1,0 }, {1,0} };

	// create the vertex buffer
	BEDXVertexBuffer* pVB = new BEDXVertexBuffer(device, &verticies, ARRAYSIZE(verticies), sizeof(BETexVertex));
	drawables.push_back(pVB);

	// create the vertex shader
	BEDXVertexShader* pVS = new BEDXVertexShaderPosTex(device, L"VSPassthrough.cso");
	pVS->Bind(device);
	resources.push_back(pVS);

	// create the pixel shader
	BEDXPixelShader* pPS = new BEDXPixelShader(device, L"PSPassthrough.cso");
	pPS->Bind(device);
	resources.push_back(pPS);
}

BEDXShowCanvas::~BEDXShowCanvas()
{
	if (pTex) delete pTex;
	if (pTexUpdt) delete pTexUpdt;
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

	for (auto d : drawables) d->Draw(device);
}
