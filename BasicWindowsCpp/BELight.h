#pragma once
#include "BEEntity.h"

using namespace DirectX;

// To Do... work out how to manage lights! Hacking away
class BELight : public BEEntity
{
public:
	XMVECTOR color = {1,1,1,1};
	virtual XMVECTOR CalculateColor(XMVECTOR normal) { return XMVectorSaturate(XMVector3Dot(normal, direction) * color); }

	inline void SetDirection(XMVECTOR _direction) { direction = XMVector3Normalize(_direction); };
	inline XMVECTOR GetDirection() { return direction; };

private:
	XMVECTOR direction = { 1,1,1,1 };
};

class BELightDirectional : public BELight
{
};

