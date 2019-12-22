#pragma once
#include "BELight.h"
#include "BETexture.h"

#define BESCENE_MAX_ENTITIES 100000
#define BESCENE_MAX_LIGHTS 100
#define BESCENE_MAX_TEXTURES 100

class BEScene
{
public:
	BEScene();
	~BEScene();

	BEEntity** entities = NULL;
	unsigned int entityCount = 0;

	BELight** lights = NULL;
	unsigned int lightCount = 0;

	BETexture** textures;
	unsigned int textureCount = 0;

	BEEntity* CreateAddEntity();
	void AddEntity(BEEntity* pEntity);

	void AddLight(BELight* pLight);

	void AddTexture(BETexture* pTexture);

	void Create();
};
