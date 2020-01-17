#include "BEModel.h"

BEModel::~BEModel()
{
}

void BEModel::Update(float deltaTime)
{
	for (BEEntity& entity : entitiesData)
	{
		if (entity.active) // to do: move inactive out of this list?
		{
			entity.Update(deltaTime);
		}
	}

}

// to do: not going to be efficient to resize vector. Not explecting to use this much in the future so not changing for now.
BEEntity* BEModel::CreateInstance(DirectX::XMFLOAT3A position)
{
	entitiesData.emplace_back(BEEntity(position));

	BEEntity* e = &entitiesData.back();
	e->material.pTextureSampler = pMesh->pTextureSampler; // to do: temp setup this way

	entities.push_back(e);

	return e;
}

void BEModel::CreateBulkInstances(unsigned int amount)
{
	if (entitiesData.size() > 0) throw "Already bulk created";

	if (entitiesData.capacity() < amount) entitiesData.reserve(amount);

	for (unsigned int i = 0; i < amount; i++)
	{
		entitiesData.emplace_back(BEEntity());
		entities.emplace_back(&entitiesData.back());
	}
}
