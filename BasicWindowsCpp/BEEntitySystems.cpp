#include "BEEntitySystems.h"
#include <random>

using namespace DirectX;

// to do: work out if this or XMVectorGetY XMVectorSetY is better?
#define GETY(v) v.m128_f32[1]
#define SETY(v,y) v.m128_f32[1] = y


void BEEntitySystem::SetPositionInGrid(BEModel* pModel, float spacing, unsigned int widthCount, unsigned int depthCount)
{
	unsigned int entityCount = pModel->GetEntityCout();
	BEEntity* pEntity = pModel->GetEntityData();

	assert(widthCount * depthCount == entityCount);

	float z = -((float)depthCount - 1.0f) * spacing / 2.0f;
	float xstart = -((float)widthCount - 1.0f) * spacing / 2.0f;

	for (unsigned int d = 0; d < depthCount; d++)
	{
		float x = xstart;
		for (unsigned int w = 0; w < widthCount; w++)
		{
			pEntity->SetPosition(x, 0.0f, z);
			x += spacing;
			pEntity++;
		}
		z += spacing;
	}
}

void BEPhysicsSystem::BasicUpdate(BEModel* pModel, float deltaTime)
{
	unsigned int entityCount = pModel->GetEntityCout();
	BEEntity* pEntity = pModel->GetEntityData();
	BEComponentPhysics* pPhysics = pModel->GetPhysicsData();

	for (unsigned int i = 0; i < entityCount; i++)
	{
		pEntity->position += pPhysics->velocity * deltaTime;
		pPhysics->velocity += pPhysics->acceleration * deltaTime;
		pEntity->rotation += pPhysics->angularVelocity * deltaTime;

		pEntity++;
		pPhysics++;
	}
}

void BEPhysicsSystem::BounceUpdate(BEModel* pModel, float deltaTime)
{
	unsigned int entityCount = pModel->GetEntityCout();
	BEEntity* pEntity = pModel->GetEntityData();
	BEComponentPhysics* pPhysics = pModel->GetPhysicsData();

	for (unsigned int i = 0; i < entityCount; i++)
	{
		if ( (GETY(pEntity->position) > 0.0f && GETY(pPhysics->acceleration) > 0.0f) ||
		     (GETY(pEntity->position) < 0.0f && GETY(pPhysics->acceleration) < 0.0f) )
			SETY(pPhysics->acceleration, -GETY(pPhysics->acceleration)); // swap acceleration direciton

		pEntity++;
		pPhysics++;
	}
}

void BEPhysicsSystem::BounceSetup(BEModel* pModel, float maxSpeed)
{
	unsigned int entityCount = pModel->GetEntityCout();
	BEComponentPhysics* pPhysics = pModel->GetPhysicsData();

	for (unsigned int i = 0; i < entityCount; i++)
	{
		float dir = maxSpeed * (i % 2 == 1 ? 1.0f : -1.0f);
		SETY(pPhysics->velocity, dir);
		SETY(pPhysics->acceleration, -dir);
		pPhysics++;
	}
}

void BEPhysicsSystem::RandomRotationSetup(BEModel* pModel, float maxSpeed)
{
	unsigned int entityCount = pModel->GetEntityCout();
	BEComponentPhysics* pPhysics = pModel->GetPhysicsData();

	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<float> dist(-maxSpeed * XM_PI, maxSpeed * XM_PI);

	for (unsigned int i = 0; i < entityCount; i++)
	{
		float x = dist(gen);
		float y = dist(gen);
		float z = dist(gen);
		pPhysics->angularVelocity = XMVECTOR({ x, y, z, 1.0f });
		pPhysics++;
	}
}
