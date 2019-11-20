#pragma once
#include "BEEntity.h"

using namespace DirectX;
using namespace SimpleMath;

class BELight : public BEEntity
{
public:
	virtual Color CalculateColor(Vector3 normal) = 0;
};

class BELightDirectional : public BELight
{
public:
	void SetDirection(Vector3 _direction) { direction = _direction; direction.Normalize(); };
	Color CalculateColor(Vector3 normal);

private:
	Vector3 direction = { 1,1,1 };
};

