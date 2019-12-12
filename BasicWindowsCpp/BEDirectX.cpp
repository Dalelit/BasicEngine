#include "BEDirectX.h"
#include <d3dcompiler.h>
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"
#include "BEDXVertexBuffer.h"
#include "BETimer.h"
#include <sstream>

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

void BEDirectX::Initialise3D()
{
	/////////////////// Vertex Shader stage
	BEDXVertexShader* pVS = new BEDXVertexShader(device, L"VertexShader.cso");
	pVS->Bind(device);
	resources.push_back(pVS);

	pVSConstantBuffer = new BEDXVSConstantBuffer(device);
	constantbuffers.push_back(pVSConstantBuffer);

	/////////////////// Pixel Shader stage
	BEDXPixelShader* pPS = new BEDXPixelShader(device, L"PixelShader.cso");
	pPS->Bind(device);
	resources.push_back(pPS);

	pPSConstantBuffer = new BEDXPSConstantBuffer(device);
	constantbuffers.push_back(pPSConstantBuffer);
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
	for (auto b : constantbuffers) b->Update(device, pScene, pCamera);

	return 0;
}

int BEDirectX::DoFrame()
{
	BETimer timer;
	timer.Start();

	device.BeginFrame();

	for (auto b : constantbuffers) b->Bind(device);

	for (auto d : drawables) d->Draw(device);

	std::wstringstream message;
	message << "Rendering time: " << timer.Tick().ElapsedMilSec() << "ms";

	overlay.Draw(message.str());

	device.PresentFrame();

	return 0;
}

void BEDirectX::ShowBitmap(BECanvas& canvas)
{
	overlay.ShowBitmap(canvas);
	device.PresentFrame();
}

void BEDirectX::ShowBuffer(BECanvas& canvas)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC1 texDesc = {};
	texDesc.Width = canvas.width;
	texDesc.Height = canvas.height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = canvas.buffer;
	data.SysMemPitch = canvas.GetCanvasPitch();

	Microsoft::WRL::ComPtr<ID3D11Texture2D1> pTexture = nullptr;

	hr = device.pDevice->CreateTexture2D1(&texDesc, &data, &pTexture);

	BEDXRESOURCE_ERRORCHECK(hr);

	device.PresentFrame();
}
