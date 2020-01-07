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

void BEScene::CreateSceneTest0()
{
	BEModel* pModel;
	BEEntity* pEntity;
	//BEPointLight* pLight;

	ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	directionalLight.color = { 0.75f,0.75f,0.75f,1.0f };
	directionalLight.SetDirection({ 4, -2, -3, 1 });

	//pLight = new BEPointLight();
	//lights.push_back(pLight);
	//pLight->position = { 2,4,2,1 };
	//pLight->color = { 1.0f,0.1f,0.1f,1.0f };

	// to do: temp inclusion to stop dx warnings... will fix when multi textures handled
	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	textures.push_back(t);
	BESampler* s = new BESampler(*t);
	samplers.push_back(s);

	pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::CubeMesh();
	//pModel->pMesh = BEMeshPrimatives::TriangleMesh();
	pModel->pMesh->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);
	//pEntity->Translate(3, 0, 0);
	//pEntity->SetScale({ 2, 2, 2, 1 });
}

void BEScene::CreateSceneTest1()
{
	BEModel* pModel;
	BEEntity* pEntity;
	BEPointLight* pLight;

	ambientLight = {0.1f,0.1f,0.1f,1.0f};
	directionalLight.color = { 1,1,1,1 };
	directionalLight.SetDirection({ -4, -2, -3, 1 });

	//pLight = new BEPointLight();
	//lights.push_back(pLight);
	//pLight->position = { 2,2,2,1 };
	//pLight->color = { 1.0f,0.1f,0.1f,1.0f };

	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	textures.push_back(t);
	BESampler* s = new BESampler(*t);
	samplers.push_back(s);

	pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::Ground(10,10,10,10,-1.5f, -0.5f);
	pModel->pMesh->pTextureSampler = s;
	entityRef.push_back(pEntity = pModel->CreateInstance());

	pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\monkey.stl");
	pModel->pMesh->SetColor({ 1,0,1,1 });
	entityRef.push_back(pEntity = pModel->CreateInstance({ -2,0,0 }));
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));

	pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh->SetColor({ 0.1f,1,1,1 });
	entityRef.push_back(pEntity = pModel->CreateInstance({ 2,0,0 }));
	pEntity->components.emplace_back(new BEEntityComponentOrbit(pEntity));
}

void BEScene::CreateSceneTest2()
{
	BEModel* pModel;
	BEEntity* pEntity;
	BEPointLight* pLight;

	ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	directionalLight.color = { 1,1,1,1 };
	directionalLight.SetDirection({ -4, -2, -3, 1 });

	pLight = new BEPointLight();
	lights.push_back(pLight);
	pLight->position = {4,4,4,1};
	pLight->color = {0.8f,0,0,1};

	// to do: temp inclusion to stop dx warnings... will fix when multi textures handled
	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	textures.push_back(t);
	BESampler* s = new BESampler(*t);
	samplers.push_back(s);

	pModel = new BEModel();
	models.push_back(pModel);
	//pModel->pMesh = BEMeshPrimatives::CubeMesh();
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh->SetColor({ 0.1f,0.5f,1.0f,1.0f });

	pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);
	//pEntity->Rotate(0, DirectX::XMConvertToRadians(45.0f), 0);
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));

	pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);
	//pEntity->Rotate(0, DirectX::XMConvertToRadians(45.0f), 0);
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));
	pEntity->Translate(2, 0, 0);
	pEntity->SetScale({0.5, 0.5, 0.5, 1});

	pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);
	//pEntity->Rotate(0, DirectX::XMConvertToRadians(45.0f), 0);
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));
	pEntity->Translate(-2, 0, 0);
	pEntity->SetScale({ 1.5, 1.5, 1.5, 1 });
}

void BEScene::CreateSceneTest3()
{
	BEModel* pModel;
	BEEntity* pEntity;
	BEPointLight* pLight;

	ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	directionalLight.color = { 0.1f,0.1f,1.0f,1.0f };
	directionalLight.SetDirection({ 4, -2, -3, 1 });

	pLight = new BEPointLight();
	lights.push_back(pLight);
	pLight->position = { 2,4,2,1 };
	pLight->color = { 1.0f,0.1f,0.1f,1.0f };

	// to do: temp inclusion to stop dx warnings... will fix when multi textures handled
	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	textures.push_back(t);
	BESampler* s = new BESampler(*t);
	samplers.push_back(s);

	pModel = new BEModel();
	models.push_back(pModel);
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh->SetColor({ 0.1f,0.5f,1.0f,1.0f });

	pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);
	//pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity, 0,-1,0));
	pEntity->Translate(3, 0, 0);
	pEntity->SetScale({ 2, 2, 2, 1 });

	pEntity = pModel->CreateInstance();
	entityRef.push_back(pEntity);
	//pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity, 0,1,0));
	pEntity->Translate(-2, 0, 0);
}

