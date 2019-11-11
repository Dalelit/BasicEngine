#include "BEWorld.h"

void BEWorld::AddModel(BEModel* pModel)
{
	assert(modelCount < BEWORLD_MAX_MODELS);

	models[modelCount] = pModel;
	modelCount++;
}


BEEntity* BEWorld::CreateAddEntity()
{
	BEEntity* e = new BEEntity();
	AddEntity(e);
	return e;
}

void BEWorld::AddEntity(BEEntity* pEntity)
{
	assert(entityCount < BEWORLD_MAX_ENTITIES);

	entities[entityCount] = pEntity;
	entityCount++;
}

void BEWorld::Create()
{
	BEEntity* e;

	e = CreateAddEntity();
	e->mesh = BEModel::AxisMesh();

	//e = CreateAddEntity();
	//e->mesh = BEModel::TriangleMesh();
	//e->Scale({ 2,2,2 });
	//e->color = {1,0,0};

	//e = CreateAddEntity();
	//e->mesh = BEModel::TriangleMesh();
	//e->Translate({ 1,0,1 });
	//e->color = { 0,1,0 };

	//e = CreateAddEntity();
	//e->mesh = BEModel::TriangleMesh();
	//e->Translate({ 0,-0.5f,-1 });
	//e->color = { 0,0,1 };

	e = CreateAddEntity();
	e->mesh = BEModel::CubeMesh();
	e->Scale(0.5f);
	//e->Translate({ 0,-0.3f,-2.0f });
	e->color = { 1,1,1 };

	e = CreateAddEntity();
	e->mesh = BEModel::CubeMesh();
	e->Scale(0.5f);
	e->Translate({ 1,0,0 });
	e->color = { 1,0,0 };

	e = CreateAddEntity();
	e->mesh = BEModel::CubeMesh();
	e->Scale(0.5f);
	e->Translate({ 0,1,0 });
	e->color = { 0,1,0 };

	e = CreateAddEntity();
	e->mesh = BEModel::CubeMesh();
	e->Scale(0.5f);
	e->Translate({ 0,0,-1 });
	e->color = { 0,0,1 };
}

