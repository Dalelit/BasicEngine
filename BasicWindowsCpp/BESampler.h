#pragma once
#include "BETexture.h"

class BESampler
{
public:
	enum BOUNDARY_MODE
	{
		blank, // use blank color
		wrap,  // wrap the image
		clamp, // use the edge pixel
		error  // debugging... throw error on edge extended... for testing
	};

	BESampler(BETexture& texture);

	inline DirectX::XMVECTOR SampleClosest(DirectX::XMFLOAT2 coord, BOUNDARY_MODE mode = BOUNDARY_MODE::blank) { return SampleClosest(coord.x, coord.y, mode); }
	DirectX::XMVECTOR SampleClosest(float u, float v, BOUNDARY_MODE mode = BOUNDARY_MODE::blank);

	DirectX::XMVECTOR blankColor = {0,0,0,1};

private:
	BETexture& texture;
	unsigned int stride;
	float height;
	float width;
	BECanvas::Color* pBuffer;

};

