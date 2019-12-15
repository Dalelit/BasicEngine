#pragma once

#include "BEVertex.h"
#include "BEDirectX.h"
#include "BEDXTextureUpdateable.h"

class BEDXShowCanvas
{
public:
	BEDXShowCanvas(BEDirectX& dx, BECanvas& canvas);
	void Draw(BEDirectX& dx);

private:
	BEDXTextureUpdateable tex;
	BETexVertex verticies[6];
};

