#pragma once
#include "BEEntity.h"
#include "BEEntityComponents.h"
#include "BEMesh.h"
#include <vector>

class BEModel
{
public:
	BEMesh* pMesh = nullptr;

	// to do: remove this reduntant pointer to the data. Here due to legacy of it.
	std::vector<BEEntity*> entities;

	inline unsigned int GetEntityCout() { return entityCount; };
	inline BEEntity* GetEntityData() { return entitiesData.data(); };
	inline BEComponentPhysics* GetPhysicsData() { return physicsData.data(); };

	~BEModel();

	void Update(float deltaTime);

	// Create individual instance.
	BEEntity* CreateInstance(DirectX::XMFLOAT3A position = {0,0,0});

	// Create a bulk amount..
	void CreateBulkInstances(unsigned int amount);

	// Add physics if we want it
	// To do: always add it by default? Thinking static things will not need it.
	void AddPhysics();

	std::vector<void (*)(BEModel * pModel, float deltaTime)> updateFunctions;

private:
	unsigned int entityCount = 0;
	std::vector<BEEntity> entitiesData;
	std::vector<BEComponentPhysics> physicsData;
};

