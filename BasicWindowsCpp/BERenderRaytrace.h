#pragma once
#include "BEScene.h"
#include "BECamera.h"
#include "BECanvas.h"

class BERenderRaytrace
{
public:
	bool exitLoop = false;    // used to exit the rendering loops early
	bool showBuffer = false;  // used to tell the main thread to show the back buffer for progressive results

	unsigned int raysToProcess;
	unsigned int raysProcessed;

	bool backfaceCull = true;

	BERenderRaytrace(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderRaytrace();

	void Draw();
	void Draw(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height);
	void DrawByLine();
	void DrawBySampling(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height);

	void ResetStats() { raysProcessed = 0; }
	void ClearCanvas() { pCanvas->Clear(); }

	inline unsigned int GetWidth() { return pCanvas->Width(); }
	inline unsigned int GetHeight() { return pCanvas->Height(); }

private:
	BEScene* pScene = nullptr;
	BECamera* pCamera = nullptr;
	BECanvas* pCanvas = nullptr;

	unsigned int stride;
	float invWidthx2;
	float invHeightx2;

	void InnerLoop(unsigned int x, unsigned int y);
};
