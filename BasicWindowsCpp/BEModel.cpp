#include "BEModel.h"

BEModel::~BEModel()
{
	for (auto e : entities) delete e;
}

BEEntity* BEModel::CreateInstance(DirectX::XMFLOAT3A position)
{
	BEEntity* e = new BEEntity(position);
	e->material.pTextureSampler = pMesh->pTextureSampler; // to do: temp setup this way

	entities.push_back(e);

	return e;
}
