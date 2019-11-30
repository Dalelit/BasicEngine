#include "BEDirectX.h"
#include <d3dcompiler.h>
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"
#include "BEDXVertexBuffer.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

BEDirectX::BEDirectX()
{
}

BEDirectX::~BEDirectX()
{
	for (auto r : resources) delete r;
	for (auto d : drawables) delete d;
	delete pConstantBuffer;
}

int BEDirectX::Initialise(HWND hwnd, unsigned int width, unsigned int height)
{
	HRESULT hr;

	/////////////////// Initialise device

	hr = device.Initialise(hwnd, width, height);

	if (FAILED(hr)) return hr;


	/////////////////// Vertex Shader stage
	BEDXVertexShader* pVS = new BEDXVertexShader(device, L"VertexShader.cso");
	pVS->Bind(device);
	resources.push_back(pVS);

	/////////////////// Pixel Shader stage
	BEDXPixelShader* pPS = new BEDXPixelShader(device, L"PixelShader.cso");
	pPS->Bind(device);
	resources.push_back(pPS);

	pConstantBuffer = new BEDXConstantBuffer(device);

	return hr;
}

int BEDirectX::LoadScene(BEScene* pScene)
{
	BEDXVertexBuffer* pVB = new BEDXVertexBuffer(device, pScene);
	drawables.push_back(pVB);

	return 0;
}

int BEDirectX::UpdateScene(BECamera* pCamera)
{
	pConstantBuffer->Update(device, pCamera);

	return 0;
}

int BEDirectX::DoFrame()
{
	device.BeginFrame();

	pConstantBuffer->Bind(device);
	for (auto d : drawables) d->Draw(device);

	device.PresentFrame();

	return 0;
}
