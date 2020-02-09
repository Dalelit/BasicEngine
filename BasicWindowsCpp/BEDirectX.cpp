#include "BEDirectX.h"
#include <d3dcompiler.h>
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"
#include "BEDXVertexBuffer.h"
#include "BEDXTexture.h"
#include "BEDXEntityPSConstantBuffer.h"
#include "BEDXEntityVSConstantBuffer.h"
#include <time.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

BEDirectX::BEDirectX() : resources(device)
{
}

int BEDirectX::Initialise(HWND hwnd, unsigned int width, unsigned int height)
{
	HRESULT hr;

	/////////////////// Initialise device

	hr = device.Initialise(hwnd, width, height);

	if (FAILED(hr)) return hr;

	overlay.Initialise(device);

	return hr;
}

int BEDirectX::LoadScene(BEScene* pScene, BECamera* pCamera)
{
	scene.Clear();
	scene.LoadScene(resources, pScene, pCamera);

	return 0;
}

int BEDirectX::DoFrame()
{
	clock_t startTime = clock();

	device.BeginFrame();

	scene.Draw(device);

	drawTime += clock() - startTime;
	frameCount++;

	if (showStats)
	{
		overlay.message << GetStats();
		overlay.Draw();
	}

	device.PresentFrame();

	return 0;
}

void BEDirectX::ShowBitmap(BECanvas& canvas)
{
	overlay.ShowBitmap(canvas);
	device.PresentFrame();
}

std::wstring BEDirectX::GetStats()
{
	std::wstringstream msg;

	msg << "Draw time: " << GetAvgDrawMS() << "ms" << std::endl;

	return msg.str();
}
