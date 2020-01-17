#pragma once
#include "BEEntity.h"
#include "BEMesh.h"
#include <vector>

class BEModel
{
public:
	BEMesh* pMesh = nullptr;

	// to do: remove this reduntant pointer to the data. Here due to legacy of it.
	std::vector<BEEntity*> entities;

	~BEModel();

	void Update(float deltaTime);

	// Create individual instance.
	BEEntity* CreateInstance(DirectX::XMFLOAT3A position = {0,0,0});

	// Create a bulk amount..
	void CreateBulkInstances(unsigned int amount);

	std::vector<BEEntity> entitiesData;
};

