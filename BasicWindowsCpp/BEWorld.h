#pragma once
#include "BEModel.h"
#include "BELight.h"

#define BEWORLD_MAX_ENTITIES 100000
#define BEWORLD_MAX_LIGHTS 100

class BEWorld
{
public:
	BEWorld();
	~BEWorld();

	BEEntity** entities = NULL;
	unsigned int entityCount = 0;

	BELight** lights = NULL;
	unsigned int lightCount = 0;

	BEEntity* CreateAddEntity();
	void AddEntity(BEEntity* pEntity);

	void AddLight(BELight* pLight);

	void Create();
};
