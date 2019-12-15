#include "BEDXShowCanvas.h"
#include "BEDXVertexBuffer.h"
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"

BEDXShowCanvas::BEDXShowCanvas(BEDirectXDevice& device, BECanvas& canvas) :
	tex(device, canvas.width, canvas.height, canvas.GetBufferPitch(), canvas.buffer, DXGI_FORMAT_R32G32B32A32_FLOAT) // texture
{
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
	BEDXVertexShader* pVS = new BEDXVertexShader(device, L"VSPassthrough.cso", BEDXVertexShader::InputLayout::POSITION3_TEXCOORD2);
	pVS->Bind(device);
	resources.push_back(pVS);

	// create the pixel shader
	BEDXPixelShader* pPS = new BEDXPixelShader(device, L"PSPassthrough.cso");
	pPS->Bind(device);
	resources.push_back(pPS);
}

void BEDXShowCanvas::Draw(BEDirectXDevice& device)
{
	tex.UpdateFromSource(device);
	tex.Bind(device);

	for (auto d : drawables) d->Draw(device);
}
