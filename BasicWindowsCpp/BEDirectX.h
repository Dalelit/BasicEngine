#pragma once
#include "BEDirectXResource.h"
#include "BEDirectXDrawable.h"
#include "BEDXOverlay.h"
#include "BEDXVSConstantBuffer.h"
#include "BEDXPSConstantBuffer.h"
#include "BEScene.h"
#include "BECamera.h"
#include "BECanvas.h"
#include <vector>
#include <wrl.h>
#include <sstream>

namespace wrl = Microsoft::WRL;

class BEDirectX
{
public:

	BEDirectXDevice device;
	BEDXOverlay overlay;

	bool showStats = true;

	BEDirectX();
	~BEDirectX();

	// To do: sort out the input parameters nicely
	int InitialiseBase(HWND hwnd, unsigned int width, unsigned int height);
	void Initialise3D(BEScene* pScene, BECamera* pCamera);

	int LoadScene(BEScene* pScene);

	int DoFrame();
	int DoFrameWithExtra(BEDirectXDrawable& toDraw);

	// show the bitmap version of the canvas. Assumes it's already stored.
	void ShowBitmap(BECanvas& canvas);

	std::wstring GetStats();
	float GetAvgDrawMS() { return (float)drawTime / (float)frameCount; }

private:
	std::vector<BEDirectXResource*> resources;
	std::vector<BEDirectXDrawable*> drawables;
	std::vector<BEDirectXConstantBuffer*> constantbuffers;

	long drawTime = 0;
	long frameCount = 0;
};
