#pragma once
#include <d3d12.h>
#include "SimpleMath.h"

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

	int width = 0;
	int height = 0;
	int size = 0;

	int Initialise(int width, int height);

	void Clear();

	void BufferToBMP();

	void DrawLineSafe(Vector3 from, Vector3 to, Color colorFrom, Color colorTo);

	inline void DrawLineSafe(Vector3 from, Vector3 to, Color color) { DrawLineSafe(from, to, color, color); };

	void Tests();

private:
	float halfWidth;
	float halfHeight;
};
