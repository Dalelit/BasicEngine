#pragma once

#include <DirectXMath.h>

struct BEVertex
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR normal;
	DirectX::XMVECTOR color;
};

struct BETexVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
};

struct BETriangle
{
	unsigned int* indx; // pointer the to uint[3]
};
