#pragma once
#include "BECommon.h"

using namespace DirectX;
using namespace SimpleMath;

class BELight
{
public:
	Color calc(Vector3 point, Vector3 normal, Color color);
};

class BELightDirectional : BELight
{
};

