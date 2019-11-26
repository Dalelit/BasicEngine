#include "BEWorld.h"

BEWorld::BEWorld()
{
	entities = new BEEntity*[BEWORLD_MAX_ENTITIES];
	lights = new BELight*[BEWORLD_MAX_LIGHTS];
}

BEWorld::~BEWorld()
{
	for (unsigned int i = 0; i < entityCount; i++) delete entities[i];
	for (unsigned int i = 0; i < lightCount; i++) delete lights[i];
	delete entities;
	delete lights;
}

BEEntity* BEWorld::CreateAddEntity()
{
	BEEntity* e = new BEEntity();
	AddEntity(e);
	return e;
}

void BEWorld::AddEntity(BEEntity* pEntity)
{
	assert(entityCount < BEWORLD_MAX_ENTITIES - 1);

	entities[entityCount] = pEntity;
	entityCount++;
}

void BEWorld::AddLight(BELight* pLight)
{
	assert(lightCount < BEWORLD_MAX_LIGHTS - 1);

	lights[lightCount] = pLight;
	lightCount++;
}

void BEWorld::Create()
{
	BEEntity* e;

	//e = CreateAddEntity();
	//e->mesh = BEModel::AxisMesh();

	e = CreateAddEntity();
	e->mesh = BEModel::TriangleMesh();
	//e->Scale({ 2,2,2 });
	e->color = {1,0,0};
	//Matrix m = Matrix::CreateFromYawPitchRoll(3.14f / 4.0f, 0, 0);
	//e->Transform(m);
	//e->mesh->CalculateNormals();

	e = CreateAddEntity();
	e->mesh = BEModel::TriangleMesh();
	e->Translate({ 0.5,0,1 });
	e->color = { 0,1,0 };

	e = CreateAddEntity();
	e->mesh = BEModel::TriangleMesh();
	e->Translate({ -0.5,0,-1 });
	e->color = { 0,0,1 };

	//e = CreateAddEntity();
	//e->mesh = BEModel::CubeMesh();
	//e->Scale(0.5f);
	//e->Translate({ 0,-0.3f,-2.0f });
	//e->color = { 1,1,1 };

	//e = CreateAddEntity();
	//e->mesh = BEModel::CubeMesh();
	//e->Scale(0.5f);
	//e->Translate({ 1,0,0 });
	//e->color = { 1,0,0 };

	//e = CreateAddEntity();
	//e->mesh = BEModel::CubeMesh();
	//e->Scale(0.5f);
	//e->Translate({ 0,1,0 });
	//e->color = { 0,1,0 };

	//e = CreateAddEntity();
	//e->mesh = BEModel::CubeMesh();
	//e->Scale(0.5f);
	//e->Translate({ 0,0,-1 });
	//e->color = { 0,0,1 };

	BELightDirectional* ld = new BELightDirectional();
	ld->color = {1,1,1,1};
	ld->SetDirection({-4, -4, -4});
	AddLight(ld);
}

