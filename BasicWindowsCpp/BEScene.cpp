#include "BEScene.h"
#include "BEMeshPrimatives.h"
#include "BEMeshLoaderSTL.h"

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

void BEScene::CreateSceneTestGround()
{
	BEModel* pModel;

	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	textures.push_back(t);

	BESampler* s = new BESampler(*t);
	samplers.push_back(s);

	pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::Ground(10,10,10,10,-1.5f, -0.5f);
	pModel->pMesh->pTextureSampler = s;
	BEEntity* pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);

	pModel = new BEModel();
	models.push_back(pModel);
	//pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\monkey.stl");
	entityRef.push_back(pModel->CreateInstance());
	pModel->pMesh->SetColor({ 1,0,1,1 });

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

