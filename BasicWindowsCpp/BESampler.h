#pragma once
#include "BETexture.h"

class BESampler
{
public:
	BESampler(BETexture& texture);

	inline XMVECTOR SampleClosest(XMFLOAT2 coord) { return SampleClosest(coord.x, coord.y); }
	XMVECTOR SampleClosest(float u, float v);

	XMVECTOR blank = {0,0,0,1};

private:
	BETexture& texture;
	unsigned int stride;
	float height;
	float width;
	BECanvas::Color* pBuffer;

};

