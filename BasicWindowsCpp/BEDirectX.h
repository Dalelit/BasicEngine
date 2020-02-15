#pragma once
#include "pch.h"
#include "BEDirectXResource.h"
#include "BEDirectXResourceManager.h"
#include "BEDXOverlay.h"
#include "BEDXVSConstantBuffer.h"
#include "BEDXPSConstantBuffer.h"
#include "BEScene.h"
#include "BECamera.h"
#include "BECanvas.h"
#include "BEDXScene.h"

namespace wrl = Microsoft::WRL;

class BEDirectX
{
public:

	BEDirectXDevice device;
	BEDXOverlay overlay;

	bool showStats = true;

	BEDirectX();

	BEDirectXResourceManager resources;

	int Initialise(HWND hwnd, unsigned int width, unsigned int height);

	int LoadScene(BEScene* pScene, BECamera* pCamera);

	int DoFrame();

	// show the bitmap version of the canvas. Assumes it's already stored.
	void ShowBitmap(BECanvas& canvas);

	std::wstring GetStats();
	float GetAvgDrawMS() { return (float)drawTime / (float)frameCount; }

private:
	BEDXScene scene;

	long drawTime = 0;
	long frameCount = 0;
};
