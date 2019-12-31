#include "BEDirectX.h"
#include <d3dcompiler.h>
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"
#include "BEDXVertexBuffer.h"
#include "BETimer.h"
#include "BEDXTexture.h"
#include "BEDXEntityPSConstantBuffer.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

BEDirectX::BEDirectX()
{
}

BEDirectX::~BEDirectX()
{
	for (auto r : resources) delete r;
	for (auto d : drawables) delete d;
	for (auto c : constantbuffers) delete c;
}

int BEDirectX::InitialiseBase(HWND hwnd, unsigned int width, unsigned int height)
{
	HRESULT hr;

	/////////////////// Initialise device

	hr = device.Initialise(hwnd, width, height);

	if (FAILED(hr)) return hr;

	overlay.Initialise(device);

	return hr;
}

void BEDirectX::Initialise3D(BEScene* pScene, BECamera* pCamera)
{
	/////////////////// Vertex Shader stage
	BEDXVertexShader* pVS = new BEDXVertexShader(device, L"VertexShader.cso", BEDXVertexShader::InputLayout::POSITION4_NORMAL4_COLOR4);
	pVS->Bind(device);
	resources.push_back(pVS);

	constantbuffers.push_back(new BEDXVSConstantBuffer(device, pScene, pCamera));

	/////////////////// Pixel Shader stage
	BEDXPixelShader* pPS = new BEDXPixelShader(device, L"PixelShader.cso");
	pPS->Bind(device);
	resources.push_back(pPS);

	constantbuffers.push_back(new BEDXPSConstantBuffer(device, pScene, pCamera));
}

int BEDirectX::LoadScene(BEScene* pScene)
{
	for (BEModel* model : pScene->models)
	{
		BEMesh* m = model->pMesh; // get it's mesh

		for (BEEntity* e : model->entities)
		{
			if (m)
			{
				BEDXEntityPSConstantBuffer* pEntityPSCB = new BEDXEntityPSConstantBuffer(device, e);
				pEntityPSCB->slot = 1u; // to do: sort this out properly

				BEDXVertexBuffer* pVB = new BEDXVertexBuffer(device, m, sizeof(BEVertex)); // to do: hard coded vertex size?

				pVB->resources.push_back(pEntityPSCB);

				drawables.push_back(pVB);
			}
		}
	}

	/////////////////// Textures
	for (auto t : pScene->textures)
	{
		BEDXTexture* pTex = new BEDXTexture(device, *t);
		pTex->Bind(device);
		resources.push_back(pTex);
	}

	return 0;
}

int BEDirectX::DoFrame()
{
	BETimer timer;

	device.BeginFrame();

	for (auto b : constantbuffers) b->Bind(device);

	for (auto d : drawables) d->Draw(device);

	overlay.message << "DX time: " << timer.Tick().ElapsedMilSec() << "ms";

	overlay.Draw();

	device.PresentFrame();

	return 0;
}

// to do: it is a cut/paste of the draw... is there a better way?
int BEDirectX::DoFrameWithExtra(BEDirectXDrawable& toDraw)
{
	BETimer timer;
	timer.Start();

	device.BeginFrame();

	for (auto b : constantbuffers) b->Bind(device);

	toDraw.Draw(device); // draw an extra object.

	for (auto d : drawables) d->Draw(device);

	overlay.message << "DX time: " << timer.Tick().ElapsedMilSec() << "ms";

	overlay.Draw();

	device.PresentFrame();

	return 0;
}

void BEDirectX::ShowBitmap(BECanvas& canvas)
{
	overlay.ShowBitmap(canvas);
	device.PresentFrame();
}
