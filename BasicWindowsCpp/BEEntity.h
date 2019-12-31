#pragma once
#include "BEMaterial.h"
#include <vector>
#include <math.h>

class BEEntity;

class BEEntityComponent
{
public:
	BEEntityComponent(BEEntity* pEntity);

	virtual void Update(float deltatime) = 0;
	BEEntity* parent = nullptr;
};

class BEEntity
{
public:
	bool active = true;
	DirectX::XMVECTOR position = { 0,0,0,1 };
	BEMaterial material; // to do: work out where this should be

	BEEntity() = default;
	BEEntity(DirectX::XMFLOAT3A _position);

	std::vector<BEEntityComponent*> components;

	inline void Update(float deltaTime) { for (BEEntityComponent* cmp : components) cmp->Update(deltaTime); };
};

