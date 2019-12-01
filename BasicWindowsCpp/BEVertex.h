#pragma once

#include <DirectXMath.h>

struct BEVertex
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR normal;
	DirectX::XMVECTOR color;
};

struct BETriangle
{
	unsigned int* indx; // pointer the to uint[3]
	//DirectX::XMVECTOR normal;
};
