#pragma once
#include "BEModel.h"


class BEEntitySystem
{
public:
	static void SetPositionInGrid(BEModel* pModel, float spacing, unsigned int widthCount, unsigned int depthCount);
};

class BEPhysicsSystem
{
public:
	static void BasicUpdate(BEModel* pModel, float deltaTime);

	static void BounceUpdate(BEModel* pModel, float deltaTime);
	static void BounceSetup(BEModel* pModel, float maxSpeed);

	static void RandomRotationSetup(BEModel* pModel, float maxSpeed);
};

