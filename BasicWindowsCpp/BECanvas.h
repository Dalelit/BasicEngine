#pragma once
#include "pch.h"
#include "BESurface2D.h"

// Wrapper of a surface with a pixel struct to make it easy to manage the target output
class BECanvas
{
public:

	// Pixel struct to make it easy to manipulate the bitmap
	struct Pixel {
		unsigned char b, g, r, a;

		void operator=(DirectX::XMVECTOR c) {
			r = (unsigned char)c.m128_f32[0];
			g = (unsigned char)c.m128_f32[1];
			b = (unsigned char)c.m128_f32[2];
			a = (unsigned char)c.m128_f32[3];
		}
	};

	std::unique_ptr<BESurface2D<Pixel>> bmpSurface;

	inline void Clear() { bmpSurface->Clear(); };

	inline unsigned int Width()  { return bmpSurface->GetWidth(); };
	inline unsigned int Height() { return bmpSurface->GetHeight(); };

	inline void Copy(BECanvas& src) { bmpSurface->Copy(src.bmpSurface.get()); }

	void Initialise(unsigned int width, unsigned int height) {
		bmpSurface = std::make_unique<BESurface2D<Pixel>>(width, height);
		Clear();
	};

};
