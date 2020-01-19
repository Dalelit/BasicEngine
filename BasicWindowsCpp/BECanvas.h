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

	BESurface2D<float>* depthBufferSurface = nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int size = 0;

	float defaultDepthValue = FLT_MAX;

	~BECanvas();

	int Initialise(unsigned int width, unsigned int height);

	void AddDepthBuffer();

	void Clear();

	void BufferToBMP();
	void BufferToBMPv2();

	void DrawLineSafe(DirectX::XMVECTOR from, DirectX::XMVECTOR to, DirectX::XMVECTOR colorFrom, DirectX::XMVECTOR colorTo);

	inline void DrawLineSafe(DirectX::XMVECTOR from, DirectX::XMVECTOR to, DirectX::XMVECTOR color) { DrawLineSafe(from, to, color, color); };

	inline DirectX::XMVECTOR ScreenToPixel(DirectX::XMVECTOR screenPoint) { return DirectX::XMVectorMultiply(DirectX::XMVectorAdd(screenPoint, x1y1z0), halfWH); }

private:
	float halfWidth = 0;
	float halfHeight = 0;
	DirectX::XMVECTOR x1y1z0 = { 1.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMVECTOR halfWH = {0,0,0,0};

	void InitialiseBitmapConversion();
	WICRect rect = {};
	Microsoft::WRL::ComPtr<IWICBitmap> pBitmap = nullptr;
	Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter = nullptr;
	Microsoft::WRL::ComPtr<IWICImagingFactory> pFactory = nullptr;
};
