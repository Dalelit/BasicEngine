#include "BEScene.h"
#include "BEMeshPrimatives.h"
#include "BEMeshLoaderSTL.h"
#include "BEEntityComponents.h"

BEScene::BEScene()
{
}

BEScene::~BEScene()
{
	for (auto m : models) delete m;
	for (auto t : textures) delete t;
	for (auto s : samplers) delete s;
	for (auto l : lights) delete l;
}

void BEScene::Update(float deltaTime)
{
	for (BEEntity* entity : entityRef)
	{
		entity->Update(deltaTime);
	}

	for (BEPointLight* light : lights)
	{
		light->Update(deltaTime);
	}
}

