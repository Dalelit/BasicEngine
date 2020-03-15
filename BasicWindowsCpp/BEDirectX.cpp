#include "pch.h"
#include "BEDirectX.h"
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"
#include "BEDXVertexBuffer.h"
#include "BEDXTexture.h"
#include "BEDXEntityPSConstantBuffer.h"
#include "BEDXEntityVSConstantBuffer.h"
#include "Submodules/imgui/imgui.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

BEDirectX::BEDirectX() : resources(device)
{
}

int BEDirectX::Initialise(HWND hwnd)
{
	HRESULT hr;

	/////////////////// Initialise device

	hr = device.Initialise(hwnd);

	if (FAILED(hr)) return hr;

	overlay.Initialise(device);

	imgui.Initialise(hwnd, device);

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

	if (imgui.IsEnabled())
	{
		imgui.Draw();
	}

	device.PresentFrame();

	return 0;
}

void BEDirectX::ShowBitmap(BECanvas& canvas)
{
	overlay.ShowBitmap(canvas);
	device.PresentFrame();
}

bool BEDirectX::IsFullScreen()
{
	return device.IsFullScreen();
}

void BEDirectX::MakeFullScreen()
{
	device.MakeFullScreen();
}

void BEDirectX::MakeWindowed()
{
	device.MakeWindowed();
}

void BEDirectX::Resize()
{
	overlay.ReleaseDeviceResources();
	device.Resize();
	overlay.CreateDeviceResources(device);
}

std::wstring BEDirectX::GetStats()
{
	std::wstringstream msg;

	msg << "Draw time: " << GetAvgDrawMS() << "ms" << std::endl;

	return msg.str();
}

void BEDirectX::ShowImguiStats(bool* pOpen)
{
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Stats", pOpen, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Draw time: %.3f ms", GetAvgDrawMS());
		ImGui::End();
	}
}
