#pragma once
#include "BECommon.h"


using namespace DirectX;
using namespace SimpleMath;

// Pixel struct to make it easy to manipulate the bitmap
union Pixel
{
	int data;
	struct {
		unsigned char b, g, r, a;
	};

	Pixel() { data = 0; }
	Pixel(unsigned int _data) { data = _data; }
	Pixel(unsigned char _r, unsigned char _g, unsigned char _b) { r = _r; b = _b; g = _g; a = 255; }
};

class BECanvas
{
public:
	Color* buffer = NULL;
	Pixel* bmp = NULL;
	float* depthBuffer = NULL;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int size = 0;

	float defaultDepthValue = FLT_MAX;

	int Initialise(unsigned int width, unsigned int height);

	void AddDepthBuffer();

	void Clear();

	void BufferToBMP();

	void DrawLineSafe(Vector3 from, Vector3 to, Color colorFrom, Color colorTo);

	inline void DrawLineSafe(Vector3 from, Vector3 to, Color color) { DrawLineSafe(from, to, color, color); };

	inline Vector3 ScreenToPixel(Vector3 screenPoint) { return (screenPoint + x1y1z0) * halfWH; }

	void Tests();

private:
	float halfWidth;
	float halfHeight;
	Vector3 x1y1z0 = { 1.0f, 1.0f, 0.0f };
	Vector3 halfWH;
};
