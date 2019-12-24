#include "BEScene.h"
#include "BEMeshPrimatives.h"

BEScene::BEScene()
{
	entities = new BEEntity*[BESCENE_MAX_ENTITIES];
	lights = new BELight*[BESCENE_MAX_LIGHTS];
	textures = new BETexture * [BESCENE_MAX_TEXTURES];
}

BEScene::~BEScene()
{
	for (unsigned int i = 0; i < entityCount; i++) delete entities[i];
	for (unsigned int i = 0; i < lightCount; i++) delete lights[i];
	for (unsigned int i = 0; i < textureCount; i++) delete textures[i];
	for (auto s : samplers) delete s;
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

void BEScene::AddTexture(BETexture* pTexture)
{
	assert(textureCount < BESCENE_MAX_TEXTURES - 1);

	textures[textureCount] = pTexture;
	textureCount++;
}

void BEScene::Create()
{
	BEEntity* e;

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::AxisMesh();

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::TriangleMesh();
	//e->mesh->verticies[0].color = { 1,0,0 };
	//e->mesh->verticies[1].color = { 0,1,0 };
	//e->mesh->verticies[2].color = { 0,0,1 };
	//e->Scale({ 2,2,1,1 });
	//e->Translate({ 0.5,0,-1 });
	//XMMATRIX m = XMMatrixRotationRollPitchYaw(3.14f / 4.0f, 0, 0);
	//e->Transform(m);
	//e->mesh->CalculateNormals();

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::TriangleMesh();
	//e->Translate({ 0.5,0,1 });
	//e->SetColor( { 0,1,0 } );

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::TriangleMesh();
	//e->Translate({ -0.5,0,-1 });
	//e->SetColor( { 0,0,1 } );

	//e = CreateAddEntity();
	//e->mesh = BEMeshPrimatives::CubeMesh();
	//e->SetColor( { 1,1,1 } );
	//e->mesh->verticies[0].color = { 1,0,0 };
	//e->mesh->verticies[4].color = { 0,1,0 };
	//e->mesh->verticies[17].color = { 0,0,1 };
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

	e = CreateAddEntity();
	e->mesh = BEMeshPrimatives::Ground(10,10,10,10,-0.5f, 0.5f);

	BETexture* t = new BETexture(L"4-sunset-over-water-focusstock.jpg");
	AddTexture(t);

	BESampler* s = new BESampler(*t);
	samplers.push_back(s);

	BELightDirectional* ld = new BELightDirectional();
	ld->color = {1,1,1,1};
	ld->SetDirection({-4, -2, -3, 1});
	AddLight(ld);
}

