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

namespace wrl = Microsoft::WRL;

class BEDirectX
{
public:

	BEDirectXDevice device;
	BEDXOverlay overlay;

	BEDirectX();
	~BEDirectX();

	// To do: sort out the input parameters nicely
	int InitialiseBase(HWND hwnd, unsigned int width, unsigned int height);
	void Initialise3D();

	int UpdateFrame(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera);

	int LoadScene(BEScene* pScene);

	int DoFrame();

	// show the bitmap version of the canvas. Assumes it's already stored.
	void ShowBitmap(BECanvas& canvas);

	// show the buffer version of the canvas.
	void ShowBuffer(BECanvas& canvas);

private:
	std::vector<BEDirectXResource*> resources;
	std::vector<BEDirectXDrawable*> drawables;
	std::vector<BEDirectXConstantBuffer*> constantbuffers;
	BEDXVSConstantBuffer* pVSConstantBuffer = nullptr;
	BEDXPSConstantBuffer* pPSConstantBuffer = nullptr;
};

