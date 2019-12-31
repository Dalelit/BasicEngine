#pragma once
#include "BESampler.h"

class BEMaterial
{
public:
	//DirectX::XMVECTOR color = {1,1,1,1};
	BESampler* pTextureSampler = nullptr;

	inline bool IsTextured() { return pTextureSampler != nullptr; }
};

