#pragma once

#include "BEDirectXDevice.h"
#include <wrl.h>

class BEDirectXResource
{
public:
	virtual void Bind(BEDirectXDevice& device) = 0;
	virtual ~BEDirectXResource() = default;
};