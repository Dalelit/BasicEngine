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
	if (pVSConstantBuffer) delete pVSConstantBuffer;
	if (pPSConstantBuffer) delete pPSConstantBuffer;
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

	pVSConstantBuffer = new BEDXVSConstantBuffer(device);

	/////////////////// Pixel Shader stage
	BEDXPixelShader* pPS = new BEDXPixelShader(device, L"PixelShader.cso");
	pPS->Bind(device);
	resources.push_back(pPS);

	pPSConstantBuffer = new BEDXPSConstantBuffer(device);

	return hr;
}

int BEDirectX::LoadScene(BEScene* pScene)
{
	for (UINT eIndx = 0; eIndx < pScene->entityCount; eIndx++)
	{
		BEMesh* m = pScene->entities[eIndx]->mesh;

		if (m)
		{
			BEDXVertexBuffer* pVB = new BEDXVertexBuffer(device, m);
			drawables.push_back(pVB);
		}
	}

	return 0;
}

int BEDirectX::UpdateFrame(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera)
{
	pVSConstantBuffer->Update(device, pScene, pCamera);
	pPSConstantBuffer->Update(device, pScene, pCamera);

	return 0;
}

int BEDirectX::DoFrame()
{
	device.BeginFrame();

	pVSConstantBuffer->Bind(device);
	pPSConstantBuffer->Bind(device);

	for (auto d : drawables) d->Draw(device);

	device.PresentFrame();

	return 0;
}
