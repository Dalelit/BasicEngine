#pragma once

#include <DirectXMath.h>

struct BEVertex
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR color;
};

struct BETriangle
{
	unsigned int* indx; // pointer the to uint[3]
	DirectX::XMVECTOR normal;
};
