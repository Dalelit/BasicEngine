#pragma once
#include "BEEntity.h"

// To Do... work out how to manage lights! Hacking away
class BELight : public BEEntity
{
public:
	DirectX::XMVECTOR color = {1,1,1,1};
	virtual DirectX::XMVECTOR CalculateColor(DirectX::XMVECTOR normal) { return DirectX::XMVectorSaturate(DirectX::XMVectorNegate(DirectX::XMVectorMultiply(DirectX::XMVector3Dot(normal, direction), color))); }

	inline void SetDirection(DirectX::XMVECTOR _direction) { direction = DirectX::XMVector3Normalize(_direction); };
	inline DirectX::XMVECTOR GetDirection() { return direction; };

private:
	DirectX::XMVECTOR direction = { 1,1,1,1 };
};

class BELightDirectional : public BELight
{
};

