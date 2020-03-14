#include "pch.h"
#include "MiniPlanet.h"
#include "BEMeshPrimatives.h"

using namespace DirectX;

void MiniPlanet::CreateScene(BEScene& scene)
{
	scene.ambientLight.color = { 0.1f,0.1f,0.1f,1.0f };
	scene.directionalLight.color = { 0.75f,0.75f,0.75f,1.0f };
	scene.directionalLight.SetDirection({ 4, -2, -3, 1 });

	BEPointLight* pLight;
	pLight = new BEPointLight();
	scene.lights.push_back(pLight);
	pLight->position = { 4,4,4,1 };
	pLight->color = { 0.5f,0.5f,0.5f,1.0f };

	BEModel* pModel = new BEModel("Land");
	pModel->pMesh = BEMeshPrimatives::Icosphere(3);
	pModel->pMesh->material.Randomise();
	pModel->pMesh->material.specularExponent = 1.0f;
	scene.models.push_back(pModel);

	// randomise the radius of the vertecies.

	float variation = 0.06f;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<float> dist(1.0f - variation, 1.0f + variation);

	BEVertex* pv = pModel->pMesh->verticies;
	for (unsigned int i = 0; i < pModel->pMesh->vertCount; i++)
	{
		pv->position *= dist(gen);
		pv->position.m128_f32[3] = 1.0f;
		pv++;
	}

	pModel->CreateInstance();

	// create the ocean
	pModel = new BEModel("Ocean");
	pModel->pMesh = BEMeshPrimatives::Icosphere(3);
	pModel->pMesh->material.Randomise();
	pModel->pMesh->material.ambientColor.m128_f32[2] = 1.0f; // blueish
	pModel->pMesh->material.diffuseColor.m128_f32[2] = 1.0f; // blueish
	scene.models.push_back(pModel);
	pModel->CreateInstance();
}
