#pragma once

#include "BEDirectXDevice.h"
#include "BEDirectXResource.h"
#include <vector>
#include <wrl.h>

class BEDirectXDrawable
{
public:
	virtual void Draw(BEDirectXDevice& device) = 0;
	virtual ~BEDirectXDrawable();

	std::vector<BEDirectXResource*> resources;
};


class BEDirectXDrawableNested : public BEDirectXDrawable
{
public:
	virtual ~BEDirectXDrawableNested();

protected:
	std::vector<BEDirectXDrawable*> drawables;
};
