#pragma once
#include "BEDirectXResource.h"
#include "BEDirectXDrawable.h"
#include "BEDXOverlay.h"
#include "BEDXVSConstantBuffer.h"
#include "BEDXPSConstantBuffer.h"
#include "BEScene.h"
#include "BECamera.h"
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
	int Initialise(HWND hwnd, unsigned int width, unsigned int height);

	int UpdateFrame(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera);

	int LoadScene(BEScene* pScene);

	int DoFrame();

private:
	std::vector<BEDirectXResource*> resources;
	std::vector<BEDirectXDrawable*> drawables;
	BEDXVSConstantBuffer* pVSConstantBuffer = nullptr;
	BEDXPSConstantBuffer* pPSConstantBuffer = nullptr;
};

