#pragma once
#include "BEDirectXResource.h"
#include "BEDirectXDrawable.h"
#include "BEDXConstantBuffer.h"
#include "BEWorld.h"
#include "BECamera.h"
#include <vector>

#include <wrl.h>

namespace wrl = Microsoft::WRL;

class BEDirectX
{
public:

	BEDirectXDevice device;

	BEDirectX();
	~BEDirectX();

	// To do: sort out the input parameters nicely
	int Initialise(HWND hwnd, unsigned int width, unsigned int height);

	int UpdateScene(BECamera* pCamera);

	int LoadScene(BEWorld* pWorld);

	int DoFrame();

private:
	std::vector<BEDirectXResource*> resources;
	std::vector<BEDirectXDrawable*> drawables;
	BEDXConstantBuffer* pConstantBuffer = nullptr;
};

