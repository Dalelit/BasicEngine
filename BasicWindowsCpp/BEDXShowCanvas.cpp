#include "BEDXShowCanvas.h"
#include "BEDXVertexBuffer.h"
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"

BEDXShowCanvas::BEDXShowCanvas(BEDirectX& dx, BECanvas& canvas) :
	tex(dx.device, canvas.width, canvas.height, canvas.GetBufferPitch(), canvas.buffer, DXGI_FORMAT_R32G32B32A32_FLOAT) // texture
{
	// set the verticies
	verticies[0] = { { -1,-1,0 }, {0,0} };
	verticies[1] = { { -1, 1,0 }, {0,1} };
	verticies[2] = { {  1, 1,0 }, {1,1} };

	verticies[3] = { { -1,-1,0 }, {0,0} };
	verticies[4] = { {  1, 1,0 }, {1,1} };
	verticies[5] = { {  1,-1,0 }, {1,0} };

	// create the vertex buffer
	BEDXVertexBuffer* pVB = new BEDXVertexBuffer(dx.device, &verticies, ARRAYSIZE(verticies), sizeof(BETexVertex));
	dx.drawables.push_back(pVB);

	// create the vertex shader
	BEDXVertexShader* pVS = new BEDXVertexShader(dx.device, L"VSPassthrough.cso", BEDXVertexShader::InputLayout::POSITION3_TEXCOORD2);
	pVS->Bind(dx.device);
	dx.resources.push_back(pVS);

	// create the pixel shader
	BEDXPixelShader* pPS = new BEDXPixelShader(dx.device, L"PSPassthrough.cso");
	pPS->Bind(dx.device);
	dx.resources.push_back(pPS);
}

void BEDXShowCanvas::Draw(BEDirectX& dx)
{
	dx.device.TurnOffDepthBuffer();
	dx.device.BeginFrame();
	tex.UpdateFromSource(dx.device);
	tex.Bind(dx.device);
	for (auto d : dx.drawables) d->Draw(dx.device);
	dx.device.PresentFrame();
}
