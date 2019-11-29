#pragma once
#include "BEEntity.h"

using namespace DirectX;

class BELight : public BEEntity
{
public:
	virtual XMVECTOR CalculateColor(XMVECTOR normal) = 0;
};

class BELightDirectional : public BELight
{
public:
	void SetDirection(XMVECTOR _direction) { direction = XMVector3Normalize(_direction); };
	XMVECTOR CalculateColor(XMVECTOR normal);

private:
	XMVECTOR direction = { 1,1,1,1 };
};

