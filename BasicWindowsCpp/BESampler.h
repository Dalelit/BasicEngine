#pragma once
#include "BETexture.h"

class BESampler
{
public:
	BESampler(BETexture& texture);

	inline DirectX::XMVECTOR SampleClosest(DirectX::XMFLOAT2 coord) { return SampleClosest(coord.x, coord.y); }
	DirectX::XMVECTOR SampleClosest(float u, float v);

	DirectX::XMVECTOR blank = {0,0,0,1};

private:
	BETexture& texture;
	unsigned int stride;
	float height;
	float width;
	BECanvas::Color* pBuffer;

};

