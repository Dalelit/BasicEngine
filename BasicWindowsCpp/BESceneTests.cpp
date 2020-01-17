#include "BESceneTests.h"
#include "BEMeshPrimatives.h"
#include "BEMeshLoaderSTL.h"
#include "BEEntityComponents.h"

void BESceneTests::CreateSceneTest0(BEScene& scene)
{
	BEModel* pModel;
	BEEntity* pEntity;
	//BEPointLight* pLight;

	scene.ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	scene.directionalLight.color = { 0.75f,0.75f,0.75f,1.0f };
	scene.directionalLight.SetDirection({ 4, -2, -3, 1 });

	//pLight = new BEPointLight();
	//lights.push_back(pLight);
	//pLight->position = { 2,4,2,1 };
	//pLight->color = { 1.0f,0.1f,0.1f,1.0f };

	// to do: temp inclusion to stop dx warnings... will fix when multi scene.textures handled
	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	scene.textures.push_back(t);
	BESampler* s = new BESampler(*t);
	scene.samplers.push_back(s);

	pModel = new BEModel();
	scene.models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::CubeMesh();
	//pModel->pMesh = BEMeshPrimatives::TriangleMesh();
	pModel->pMesh->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	pEntity = pModel->CreateInstance();
	//pEntity->Translate(3, 0, 0);
	//pEntity->SetScale({ 2, 2, 2, 1 });
}

void BESceneTests::CreateSceneTest1(BEScene& scene)
{
	BEModel* pModel;
	BEEntity* pEntity;
	//BEPointLight* pLight;

	scene.ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	scene.directionalLight.color = { 1,1,1,1 };
	scene.directionalLight.SetDirection({ -4, -2, -3, 1 });

	//pLight = new BEPointLight();
	//lights.push_back(pLight);
	//pLight->position = { 2,2,2,1 };
	//pLight->color = { 1.0f,0.1f,0.1f,1.0f };

	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	scene.textures.push_back(t);
	BESampler* s = new BESampler(*t);
	scene.samplers.push_back(s);

	pModel = new BEModel();
	scene.models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::Ground(10, 10, 10, 10, -1.5f, -0.5f);
	pModel->pMesh->pTextureSampler = s;
	pEntity = pModel->CreateInstance();

	pModel = new BEModel();
	scene.models.push_back(pModel);
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\monkey.stl");
	pModel->pMesh->SetColor({ 1,0,0,1 });
	pEntity = pModel->CreateInstance({ -2,0,0 });
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));

	pModel = new BEModel();
	scene.models.push_back(pModel);
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh->SetColor({ 252.0f / 255.0f , 194.0f / 255.0f , 0.0f, 1.0f });
	pEntity = pModel->CreateInstance({ 2,0,0 });
	pEntity->components.emplace_back(new BEEntityComponentOrbit(pEntity));
}

void BESceneTests::CreateSceneTest2(BEScene& scene)
{
	BEModel* pModel;
	BEEntity* pEntity;
	BEPointLight* pLight;

	scene.ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	scene.directionalLight.color = { 1,1,1,1 };
	scene.directionalLight.SetDirection({ -4, -2, -3, 1 });

	pLight = new BEPointLight();
	scene.lights.push_back(pLight);
	pLight->position = { 4,4,4,1 };
	pLight->color = { 0.8f,0,0,1 };

	// to do: temp inclusion to stop dx warnings... will fix when multi scene.textures handled
	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	scene.textures.push_back(t);
	BESampler* s = new BESampler(*t);
	scene.samplers.push_back(s);

	pModel = new BEModel();
	scene.models.push_back(pModel);
	//pModel->pMesh = BEMeshPrimatives::CubeMesh();
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh->SetColor({ 0.1f,0.5f,1.0f,1.0f });

	pEntity = pModel->CreateInstance();
	//pEntity->Rotate(0, DirectX::XMConvertToRadians(45.0f), 0);
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));

	pEntity = pModel->CreateInstance();
	//pEntity->Rotate(0, DirectX::XMConvertToRadians(45.0f), 0);
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));
	pEntity->Translate(2, 0, 0);
	pEntity->SetScale({ 0.5, 0.5, 0.5, 1 });

	pEntity = pModel->CreateInstance();
	//pEntity->Rotate(0, DirectX::XMConvertToRadians(45.0f), 0);
	pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity));
	pEntity->Translate(-2, 0, 0);
	pEntity->SetScale({ 1.5, 1.5, 1.5, 1 });
}

void BESceneTests::CreateSceneTest3(BEScene& scene)
{
	BEModel* pModel;
	BEEntity* pEntity;
	BEPointLight* pLight;

	scene.ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	scene.directionalLight.color = { 0.1f,0.1f,1.0f,1.0f };
	scene.directionalLight.SetDirection({ 4, -2, -3, 1 });

	pLight = new BEPointLight();
	scene.lights.push_back(pLight);
	pLight->position = { 2,4,2,1 };
	pLight->color = { 1.0f,0.1f,0.1f,1.0f };

	// to do: temp inclusion to stop dx warnings... will fix when multi scene.textures handled
	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	scene.textures.push_back(t);
	BESampler* s = new BESampler(*t);
	scene.samplers.push_back(s);

	pModel = new BEModel();
	scene.models.push_back(pModel);
	pModel->pMesh = BEMeshLoaderSTL::LoadSTL(L"STL\\torus.stl");
	pModel->pMesh->SetColor({ 0.1f,0.5f,1.0f,1.0f });

	pEntity = pModel->CreateInstance();
	//pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity, 0,-1,0));
	pEntity->Translate(3, 0, 0);
	pEntity->SetScale({ 2, 2, 2, 1 });

	pEntity = pModel->CreateInstance();
	//pEntity->components.emplace_back(new BEEntityComponentSpin(pEntity, 0,1,0));
	pEntity->Translate(-2, 0, 0);
}

void BESceneTests::CreateBoxWorld(BEScene& scene, BECamera& camera)
{
	BEModel* pModel;
	//BEEntity* pEntity;

	camera.SetPosition(0, 4, 5);
	camera.LookAt(0, 0, 0);

	scene.ambientLight = { 0.1f,0.1f,0.1f,1.0f };
	scene.directionalLight.color = { 0.75f,0.75f,0.75f,1.0f };
	scene.directionalLight.SetDirection({ 4, -2, -3, 1 });

	// to do: temp inclusion to stop dx warnings... will fix when multi scene.textures handled
	BETexture* t = new BETexture(L"Textures\\4-sunset-over-water-focusstock.jpg");
	scene.textures.push_back(t);
	BESampler* s = new BESampler(*t);
	scene.samplers.push_back(s);

	pModel = new BEModel();
	scene.models.push_back(pModel);
	pModel->pMesh = BEMeshPrimatives::CubeMesh();
	pModel->pMesh->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	int cw = 5;
	int cd = 5;
	pModel->CreateBulkInstances(cw * cd);

	float width = 8.0f;
	float depth = 8.0f;

	float z = -depth;
	int indx = 0;
	for (int d = 0; d < cd; d++)
	{
		float x = -width / 2.0f;
		for (int w = 0; w < cw; w++)
		{
			pModel->entitiesData[indx].SetPosition(x, 0.0f, z);
			x += width / (cw - 1.0f);
			indx++;
		}
		z += depth / (cd - 1.0f);
	}
}
