#pragma once
#include "BEEntity.h"
#include "BEMesh.h"
#include <vector>

class BEModel
{
public:
	BEMesh* pMesh = nullptr;

	std::vector<BEEntity*> entities;

	~BEModel();

	BEEntity* CreateInstance(DirectX::XMFLOAT3A position = {0,0,0});
};

