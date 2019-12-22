#pragma once

#include "BEVertex.h"
#include "BEDirectX.h"
#include "BEDirectXDrawable.h"
#include "BEDXTextureUpdateable.h"

// To do
// - updateable bool and 2 pointers is a hack for now
//

class BEDXShowCanvas : public BEDirectXDrawableNested
{
public:
	BEDXShowCanvas(BEDirectXDevice& device, BECanvas& canvas, bool updateable = false);
	~BEDXShowCanvas();
	void Draw(BEDirectXDevice& device);

private:
	BEDXTexture* pTex = nullptr;
	BEDXTextureUpdateable* pTexUpdt = nullptr;
	BETexVertex verticies[6];
};

