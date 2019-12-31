#include "BESampler.h"

using namespace DirectX;

BESampler::BESampler(BETexture& texture) :
	texture(texture)
{
	stride = texture.canvas.GetBufferPitch();
	height = (float)texture.canvas.height;
	width = (float)texture.canvas.width;
	pBuffer = texture.canvas.buffer;
}

XMVECTOR BESampler::SampleClosest(float u, float v, BOUNDARY_MODE mode)
{
	switch (mode)
	{
		case BOUNDARY_MODE::blank :
			if (u < 0.0f || u >= 1.0f || v < 0.0f || v >= 1.0f) return blankColor;
			break;
		case BOUNDARY_MODE::wrap :
		case BOUNDARY_MODE::clamp :
		case BOUNDARY_MODE::error :
			throw "Not implemented";
	}

	unsigned int x = (int)(u * width);
	unsigned int y = (int)(v * height);

	return pBuffer[y * texture.canvas.width + x].data;
}
