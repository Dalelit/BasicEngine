#pragma once
#include "BEWorld.h"
#include "BECamera.h"
#include "BECanvas.h"

using namespace DirectX;
using namespace SimpleMath;

class BERenderPipeline
{
public:
	BEWorld* pWorld;
	BECamera* pCamera;
	BECanvas* pCanvas;

	BERenderPipeline(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);

	void UpdateScreenSpace();
	void Draw();

	void DrawModel(BEModel* pModel);
};

