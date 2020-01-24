#pragma once
#include "BESurface2D.h"
#include <wincodec.h>
#include <wrl.h>
#include <DirectXMath.h>

// Pixel struct to make it easy to manipulate the bitmap
class BECanvas
{
public:
	typedef DirectX::XMVECTOR Color;

	struct Pixel {
		unsigned char b, g, r, a;

		void operator=(DirectX::XMVECTOR& c) {
			r = (unsigned char)c.m128_f32[0];
			g = (unsigned char)c.m128_f32[1];
			b = (unsigned char)c.m128_f32[2];
			a = (unsigned char)c.m128_f32[3];
		}
	};

	BESurface2D<Pixel>* bmpSurface = nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int size = 0;

	~BECanvas();

	int Initialise(unsigned int width, unsigned int height);

	void Clear();
};
