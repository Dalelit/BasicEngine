#pragma once
#include "BESurface2D.h"
#include <wincodec.h>
#include <wrl.h>
#include <DirectXMath.h>

// Pixel struct to make it easy to manipulate the bitmap
class BECanvas
{
public:
	struct Pixel {
		unsigned char b, g, r, a;
	};

	typedef DirectX::XMVECTOR Color;

	BESurface2D<Color>* bufferSurface = nullptr;

	BESurface2D<Pixel>* bmpSurface = nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int size = 0;

	~BECanvas();

	int Initialise(unsigned int width, unsigned int height);

	void Clear();

	void BufferToBMP();
	void BufferToBMPv2();

private:
	void InitialiseBitmapConversion();
	WICRect rect = {};
	Microsoft::WRL::ComPtr<IWICBitmap> pBitmap = nullptr;
	Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter = nullptr;
	Microsoft::WRL::ComPtr<IWICImagingFactory> pFactory = nullptr;
};
