#pragma once

#include "BEDirectXDevice.h"
#include <wrl.h>

class BEDirectXDrawable
{
public:
	virtual void Draw(BEDirectXDevice& device) = 0;
	virtual ~BEDirectXDrawable() = default;
};