#pragma once

#include "BEDirectXDevice.h"
#include "BEDirectXResource.h"
#include <vector>
#include <wrl.h>

class BEDirectXDrawableResource : BEDirectXResource
{
public:
	virtual void Draw(BEDirectXDevice& device) = 0;

	std::vector<BEDirectXResource*> resources;
};

class BEDirectXDrawable
{
public:
	virtual void Draw(BEDirectXDevice& device) = 0;
};
