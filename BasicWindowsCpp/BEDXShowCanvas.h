#pragma once

#include "BEVertex.h"
#include "BEDirectX.h"
#include "BEDirectXDrawable.h"
#include "BEDXTextureUpdateable.h"

class BEDXShowCanvas : public BEDirectXDrawableNested
{
public:
	BEDXShowCanvas(BEDirectXDevice& device, BECanvas& canvas);
	void Draw(BEDirectXDevice& device);

private:
	BEDXTextureUpdateable tex;
	BETexVertex verticies[6];
};

