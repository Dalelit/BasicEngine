#include "BEScene.h"
#include "BEMeshPrimatives.h"

BEScene::BEScene()
{
	entities = new BEEntity*[BESCENE_MAX_ENTITIES];
	lights = new BELight*[BESCENE_MAX_LIGHTS];
}

BEScene::~BEScene()
{
	for (unsigned int i = 0; i < entityCount; i++) delete entities[i];
	for (unsigned int i = 0; i < lightCount; i++) delete lights[i];
	delete entities;
	delete lights;
}

BEEntity* BEScene::CreateAddEntity()
{
	BEEntity* e = new BEEntity();
	AddEntity(e);
	return e;
}

void BEScene::AddEntity(BEEntity* pEntity)
{
	assert(entityCount < BESCENE_MAX_ENTITIES - 1);

	entities[entityCount] = pEntity;
	entityCount++;
}

void BEScene::AddLight(BELight* pLight)
{
	assert(lightCount < BESCENE_MAX_LIGHTS - 1);

	lights[lightCount] = pLight;
	lightCount++;
}

void BEScene::Create()
{
	BEEntity* e;

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::AxisMesh();

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::TriangleMesh();
	//e->color = { 1,0,0 };
	//e->Scale({ 2,2,2 });
	//XMMATRIX m = XMMatrixRotationRollPitchYaw(3.14f / 4.0f, 0, 0);
	//e->Transform(m);
	//e->mesh->CalculateNormals();

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::TriangleMesh();
	//e->Translate({ 0.5,0,1 });
	//e->color = { 0,1,0 };

	e = CreateAddEntity();
	e->mesh = BEMeshPrimatives::TriangleMesh();
	e->Translate({ -0.5,0,-1 });
	e->color = { 0,0,1 };

	e = CreateAddEntity();
	e->mesh = BEMeshPrimatives::CubeMesh();
	e->color = { 1,1,1 };
	//e->Scale(0.5f);
	//e->Translate({ 0,-0.3f,-2.0f });

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::CubeMesh();
	//e->Scale(0.5f);
	//e->Translate({ 1,0,0 });
	//e->color = { 1,0,0 };

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::CubeMesh();
	//e->Scale(0.5f);
	//e->Translate({ 0,1,0 });
	//e->color = { 0,1,0 };

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::CubeMesh();
	//e->Scale(0.5f);
	//e->Translate({ 0,0,-1 });
	//e->color = { 0,0,1 };

	BELightDirectional* ld = new BELightDirectional();
	ld->color = {1,1,1,1};
	ld->SetDirection({-4, -4, -4});
	AddLight(ld);
}

