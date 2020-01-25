#pragma once
#include "BESurface2D.h"
#include <wincodec.h>
#include <wrl.h>
#include <DirectXMath.h>

// Wrapper of a surface with a pixel struct to make it easy to manage the target output
class BECanvas
{
public:

	// Pixel struct to make it easy to manipulate the bitmap
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

	~BECanvas() { if (bmpSurface) delete bmpSurface; };

	inline void Clear() { bmpSurface->Clear(); };

	inline unsigned int Width() { return bmpSurface->GetWidth(); };
	inline unsigned int Height() { return bmpSurface->GetHeight(); };

	void Initialise(unsigned int width, unsigned int height) {

		if (bmpSurface) delete bmpSurface;

		bmpSurface = new BESurface2D<Pixel>(width, height);

		Clear();
	};

};
