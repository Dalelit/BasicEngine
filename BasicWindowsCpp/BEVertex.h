#pragma once

#include <DirectXMath.h>

struct BEVertex
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR normal;
	DirectX::XMVECTOR color;
	DirectX::XMFLOAT2 texcoord;
};

struct BETexVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
};
