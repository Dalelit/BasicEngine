#include "BECanvas.h"
#include <memory.h>
#include <utility>

using namespace DirectX;

#define BE_HR_CHECK(hr) if (FAILED(hr)) throw hr; // to do: proper error checking

BECanvas::~BECanvas()
{
	if (bufferSurface) delete bufferSurface;
	if (bmpSurface) delete bmpSurface;
}

int BECanvas::Initialise(unsigned int _width, unsigned int _height)
{
	if (bufferSurface) delete bufferSurface;
	if (bmpSurface) delete bmpSurface;

	width = _width;
	height = _height;
	size = width * height;

	bufferSurface = new BESurface2D<Color>(width, height);

	bmpSurface = new BESurface2D<Pixel>(width, height);

	Clear();

	//InitialiseBitmapConversion(); // not use toBMPv2 at the moment

	return 0; // TO DO - no error checking for memory
}

void BECanvas::Clear()
{
	bufferSurface->Clear();
	//bmpSurface->Clear(); // don't need to clear, it gets written over in BufferToBMP
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
