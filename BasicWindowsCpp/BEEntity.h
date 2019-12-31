#pragma once
#include "BEMesh.h"
#include "BEMaterial.h"

class BEEntity
{
public:
	bool alive = true;
	DirectX::XMVECTOR position = { 0,0,0,0 };
	BEMaterial material; // to do: work out where this should be

	BEEntity() = default;
	BEEntity(DirectX::XMFLOAT3A _position);
};

