#pragma once
#include "BEModel.h"

using namespace DirectX;
using namespace SimpleMath;

#define BEWORLD_MAX_MODELS 100000
#define BEWORLD_MAX_ENTITIES 100000

class BEWorld
{
public:
	BEModel* models[BEWORLD_MAX_MODELS] = {};
	unsigned int modelCount = 0;
	void AddModel(BEModel* pModel);

	BEEntity* entities[BEWORLD_MAX_ENTITIES] = {};
	unsigned int entityCount = 0;

	BEEntity* CreateAddEntity();
	void AddEntity(BEEntity* pEntity);

	void Create();
	void CreateV1();
};

