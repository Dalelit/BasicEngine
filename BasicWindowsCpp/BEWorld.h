#pragma once
#include "BEModel.h"

using namespace DirectX;
using namespace SimpleMath;

#define BEWORLD_MAX_MODELS 100000

class BEWorld
{
public:
	BEModel* models[BEWORLD_MAX_MODELS] = {};
	unsigned int modelCount = 0;

	void AddModel(BEModel* pModel);

	void Create();
};

