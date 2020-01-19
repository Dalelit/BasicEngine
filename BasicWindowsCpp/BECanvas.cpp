#include "BECanvas.h"
#include <memory.h>
#include <utility>

using namespace DirectX;

#define BE_HR_CHECK(hr) if (FAILED(hr)) throw hr; // to do: proper error checking

BECanvas::~BECanvas()
{
	if (bufferSurface) delete bufferSurface;
	if (bmpSurface) delete bmpSurface;
	if (depthBufferSurface) delete depthBufferSurface;
}

int BECanvas::Initialise(unsigned int _width, unsigned int _height)
{
	if (bufferSurface) delete bufferSurface;
	if (bmpSurface) delete bmpSurface;

	width = _width;
	height = _height;
	halfWidth = floorf(width * 0.5f);
	halfHeight = floorf(height * 0.5f);
	halfWH = { halfWidth, halfHeight, 1.0f };

	size = width * height;

	bufferSurface = new BESurface2D<Color>(width, height);

	bmpSurface = new BESurface2D<Pixel>(width, height);

	if (depthBufferSurface)
	{
		delete depthBufferSurface;
		AddDepthBuffer();
	}

	Clear();

	InitialiseBitmapConversion();

	return 0; // TO DO - no error checking for memory
}

void BECanvas::Clear()
{
	bufferSurface->Clear();
	bmpSurface->Clear();

	if (depthBufferSurface) depthBufferSurface->Clear(defaultDepthValue);
}

void BECanvas::AddDepthBuffer()
{
	if (depthBufferSurface != nullptr) delete depthBufferSurface;

	depthBufferSurface = new BESurface2D<float>(width, height);

	depthBufferSurface->Clear(defaultDepthValue);
}

void BECanvas::BufferToBMP()
{
	Pixel* pp = bmpSurface->GetData();
	Color* pc = bufferSurface->GetData();
	Color c;

	#pragma loop(hint_parallel(8))
	for (unsigned int y = 0; y < size; y += width)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			c = (*pc) * 255.0f; // to do: this line is slow

			pp->r = (unsigned char)c.m128_f32[0];
			pp->g = (unsigned char)c.m128_f32[1];
			pp->b = (unsigned char)c.m128_f32[2];
			pp->a = (unsigned char)c.m128_f32[3];

			pp++;
			pc++;
		}
	}
}

// to do: color isn't being converted properly
void BECanvas::BufferToBMPv2()
{
	HRESULT hr;

	IWICBitmapLock* pLock = nullptr; // reminder - not using comptr as need to release it earlier

	hr = pBitmap->Lock(
		&rect,
		WICBitmapLockWrite,
		&pLock);

	BE_HR_CHECK(hr);

	UINT bufferSize = 0;
	BYTE* wicBmp = nullptr;

	hr = pLock->GetDataPointer(
		&bufferSize,
		&wicBmp);

	BE_HR_CHECK(hr);

	CopyMemory(wicBmp, bufferSurface->GetData(), bufferSize);

	pLock->Release();

	hr = pConverter->CopyPixels(
		nullptr,
		bmpSurface->GetPitchBytes(),
		bmpSurface->GetTotalBytes(),
		(byte*)bmpSurface->GetData());

	BE_HR_CHECK(hr);
}

void BECanvas::InitialiseBitmapConversion()
{
	rect = { 0, 0, (int)width, (int)height };

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory)
	);

	BE_HR_CHECK(hr);

	hr = pFactory->CreateBitmap(
		width, height,
		GUID_WICPixelFormat128bppRGBAFloat,
		WICBitmapCacheOnDemand,
		&pBitmap);

	BE_HR_CHECK(hr);

	hr = pFactory->CreateFormatConverter(
		&pConverter);

	BE_HR_CHECK(hr);

	hr = pConverter->Initialize(pBitmap.Get(),
		GUID_WICPixelFormat32bppBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);

	BE_HR_CHECK(hr);
}

void BECanvas::DrawLineSafe(XMVECTOR from, XMVECTOR to, XMVECTOR colorFrom, XMVECTOR colorTo)
{
	float fromX = XMVectorGetX(from);
	float fromY = XMVectorGetY(from);
	float toX = XMVectorGetX(to);
	float toY = XMVectorGetY(to);

	if (fromX < -1.0f && toX < -1.0f) return;
	if (fromY < -1.0f && toY < -1.0f) return;
	if (fromX >= 1.0f && toX >= 1.0f) return;
	if (fromY >= 1.0f && toY >= 1.0f) return;

	// convert screen space to pixels
	float x = (fromX + 1.0f) * halfWidth;
	float xt = (toX + 1.0f) * halfWidth;
	float y = (fromY + 1.0f) * halfHeight;
	float yt = (toY + 1.0f) * halfHeight;
	Color c = colorFrom;
	Color ct = colorTo;

	float dx = xt - x;
	float dy = yt - y;
	Color dc = colorTo - colorFrom;

	// special case
	// To do - optimise later
	if (fabsf(dx) < 0.5f) // change in x is less than 1 pixel... so a vertical line
	{
		if (fabsf(dy) < 0.5f) // draw a dot
		{
			bufferSurface->SetValue((int)x, (int)y, c);
			return;
		}

		dc = dc / fabsf(dy);

		if (dy > 0.0f)
		{

			if (y < 0.0f) y = 0.0f;  // To Do - fix color to start with jump

			while (y <= yt && y < height)
			{
				bufferSurface->SetValue((int)x, (int)y, c);
				y += 1.0f;
				c = c + dc;
			}
		}
		else if (dy < 0.0f)
		{
			if (y >= height) y = height - 1.0f;  // To Do - fix color to start with jump

			while (y >= yt && y >= 0.0f)
			{
				bufferSurface->SetValue((int)x, (int)y, c);
				y -= 1.0f;
				c = c + dc;
			}
		}
		return;
	}

	if (dx < 0) // swap x direction if not left to right
	{
		std::swap(x, xt);
		dx = -dx;
		std::swap(y, yt);
		dy = -dy;
		{Color temp = c; c = ct; ct = temp; }// to do : sort out swap!
		dc = -dc;
	}

	// special case
	// To do - optimise later
	if (fabsf(dy) < 0.5f) // change in y is less than 1 pixel, so horizontal
	{
		dc = dc / dx;

		if (x < 0.0f) x = 0.0f;  // To Do - fix color to start with jump

		while (x <= xt && x < width)
		{
			bufferSurface->SetValue((int)x, (int)y, c);
			x += 1.0f;
			c = c + dc;
		}
		return;
	}

	// set increment based on longest change
	if (dx > fabsf(dy))
	{
		dy /= dx;
		dc = dc / dx;
		dx = 1.0f;
	}
	else
	{
		dx /= fabsf(dy);
		dc = dc / fabsf(dy);
		dy = (dy > 0.0f ? 1.0f : -1.0f);
	}

	// move start point to 0.0 if off the left of the screen
	if (x < 0.0f)
	{
		// To Do - fix color to start with jump
		y += (-x) * dy; // move y along by the distance x needs to go
		x = 0.0f;
	}

	// move start point to 0.0 if off the bottom of the screen
	if (y < 0.0f)
	{
		// To Do - fix color to start with jump
		x += (-y) * dx;
		y = 0.0f;
	}

	// move start point to height if off the top of the screen
	if (y >= height)
	{
		// To Do - fix color to start with jump
		float yold = y;
		y = height - 1.0f;
		x += (yold - y) * dx;
	}

	// and finally pump out pixels
	while (x <= xt && x < width && y >= 0.0f && y < height)
	{
		bufferSurface->SetValue((int)x, (int)y, c);
		x += dx;
		y += dy;
		c = c + dc;
	}
}
