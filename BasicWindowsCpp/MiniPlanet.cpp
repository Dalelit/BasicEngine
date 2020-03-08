#include "pch.h"
#include "MiniPlanet.h"
#include "BEMeshPrimatives.h"

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

	BEModel* pModel = new BEModel();;
	pModel->pMesh = BEMeshPrimatives::Icosphere(3);
	pModel->pMesh->material.Randomise();
	scene.models.push_back(pModel);

	pModel->CreateInstance();
}
