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
}

void BEScene::CreateSceneTestGround()
{
	BEModel* pModel;
	BEEntity* pEntity;

	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	textures.push_back(t);

	BESampler* s = new BESampler(*t);
	samplers.push_back(s);

	pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::Ground(10,10,10,10,-1.5f, -0.5f);
	pModel->pMesh->pTextureSampler = s;
	pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);

	pModel = new BEModel();
	models.push_back(pModel);
	//pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\monkey.stl");
	pModel->pMesh->SetColor({ 1,0,1,1 });
	entityRef.push_back(pEntity = pModel->CreateInstance({ 2,0,0 }));
	pEntity->components.emplace_back(new BEEntityComponentOrbit(pEntity));
	entityRef.push_back(pModel->CreateInstance({ -2,0,0 }));

	BELightDirectional* ld = new BELightDirectional();
	ld->color = { 1,1,1,1 };
	ld->SetDirection({ -4, -2, -3, 1 });
	lights.push_back(ld);
}

void BEScene::CreateSceneTestCube()
{
	BEModel* pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::CubeMesh();

	entityRef.push_back(pModel->CreateInstance());

	BELightDirectional* ld = new BELightDirectional();
	ld->color = { 1,1,1,1 };
	ld->SetDirection({ -4, -2, -3, 1 });
	lights.push_back(ld);
}

