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
	e->mesh = BEModel::TriangleMesh();
	e->Scale({ 2,2,2 });
	e->color = {1,0,0};

	e = CreateAddEntity();
	e->mesh = BEModel::TriangleMesh();
	e->Translate({ 1,0,1 });
	e->color = { 0,1,0 };

	e = CreateAddEntity();
	e->mesh = BEModel::TriangleMesh();
	e->Translate({ 0,-0.5,-1 });
	e->color = { 0,0,1 };
}

void BEWorld::CreateV1()
{
	BEModel* tri1 = BEModel::TrianglePrimative();
	tri1->Translate({ 1.5f,0.0f,0.0f });
	AddModel(tri1);

	BEModel* tri2 = BEModel::TrianglePrimative();
	tri2->Translate({ 1.5f,0.0f,2.0f });
	tri2->color = { 1,1,0,1 };
	tri2->CreateColorData();
	tri2->colors[1] = { 1,0,1,1 };
	tri2->colors[2] = { 1,0,0,1 };
	AddModel(tri2);

	BEModel* axis = BEModel::AxisPrimative();
	AddModel(axis);

	BEModel* cube1 = BEModel::CubePrimative();
	AddModel(cube1);
}