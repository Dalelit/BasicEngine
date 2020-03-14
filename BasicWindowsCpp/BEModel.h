#pragma once
#include "pch.h"
#include "BEEntity.h"
#include "BEEntityComponents.h"
#include "BEMesh.h"

class BEModel
{
public:
	std::string name;

	BEMesh* pMesh = nullptr;

	std::vector<BEMaterial> materials;
	std::vector<BEEntity> entities;

	inline unsigned int GetEntityCout() { return (unsigned int)entities.size(); };
	inline BEEntity* GetEntityData() { return entities.data(); };
	inline BEComponentPhysics* GetPhysicsData() { return physicsData.data(); };

	BEModel();
	BEModel(std::string name);
	~BEModel();

	void Update(float deltaTime);

	// Create individual instance.
	BEEntity* CreateInstance(DirectX::XMFLOAT3A position = {0,0,0});

	// Create a bulk amount..
	void CreateBulkInstances(unsigned int amount);

	// Reserve space for entities
	void ReserveInstances(unsigned int amount);

	// Add physics if we want it
	// To do: always add it by default? Thinking static things will not need it.
	void AddPhysics();

	std::vector<void (*)(BEModel * pModel, float deltaTime)> updateFunctions;

	void ShowImguiTreeNode();

private:
	static unsigned int modelCount;
	std::vector<BEComponentPhysics> physicsData;
};

