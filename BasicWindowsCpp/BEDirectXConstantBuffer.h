#pragma once

#include "BEDirectXResource.h"
#include "BECamera.h"
#include "BEScene.h"

class BEDirectXConstantBuffer : public BEDirectXResource
{
public:
	virtual void Update(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera) = 0;
};