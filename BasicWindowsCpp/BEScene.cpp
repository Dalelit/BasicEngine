#include "pch.h"
#include "BEScene.h"
#include "BEMeshPrimatives.h"
#include "BEMeshLoaderSTL.h"
#include "BEEntityComponents.h"

BEScene::BEScene()
{
}

BEScene::~BEScene()
{
	Clear();
}

void BEScene::Update(float deltaTime)
{
	for (BEModel* m : models)
	{
		m->Update(deltaTime);
	}

	for (BEPointLight* light : lights)
	{
		light->Update(deltaTime);
	}
}

void BEScene::Clear()
{
	for (auto m : models) delete m;
	models.clear();
	for (auto t : textures) delete t;
	textures.clear();
	for (auto s : samplers) delete s;
	samplers.clear();
	for (auto l : lights) delete l;
	lights.clear();
}

