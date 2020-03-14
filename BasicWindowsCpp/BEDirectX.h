#pragma once
#include "pch.h"
#include "BEDirectXResource.h"
#include "BEDirectXResourceManager.h"
#include "BEDXOverlay.h"
#include "BEDXimgui.h"
#include "BEDXVSConstantBuffer.h"
#include "BEDXPSConstantBuffer.h"
#include "BEScene.h"
#include "BECamera.h"
#include "BECanvas.h"
#include "BEDXScene.h"

class BEDirectX
{
public:

	BEDirectXDevice device;
	BEDXOverlay overlay;
	BEDXimgui imgui;

	bool showStats = true;

	BEDirectX();

	BEDirectXResourceManager resources;

	int Initialise(HWND hwnd);

	int LoadScene(BEScene* pScene, BECamera* pCamera);

	int DoFrame();

	// show the bitmap version of the canvas. Assumes it's already stored.
	void ShowBitmap(BECanvas& canvas);

	bool IsFullScreen();
	void MakeFullScreen();
	void MakeWindowed();
	void Resize();

	std::wstring GetStats();
	float GetAvgDrawMS() { return (float)drawTime / (float)frameCount; }

	void ShowImguiStats(bool* pOpen = nullptr);

private:
	BEDXScene scene;

	long drawTime = 0;
	long frameCount = 0;
};
