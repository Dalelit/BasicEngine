#include "pch.h"
#include "BEDXOverlay.h"

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

// to do: fix the error checking macro

int BEDXOverlay::Initialise(BEDirectXDevice& dx)
{
	HRESULT hr;

	// create the factory

	D2D1_FACTORY_OPTIONS factoryOptions = {};
	factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_WARNING;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory3),
		&factoryOptions,
		&pFactory);

	BEDXRESOURCE_ERRORCHECK(hr);

	CreateDeviceResources(dx);

	hr = InitialiseTextWrite();

	BEDXRESOURCE_ERRORCHECK(hr);

	return hr;
}

void BEDXOverlay::ReleaseDeviceResources()
{
	pDevice.Reset();
	pContext.Reset();
	pBitmap.Reset();
	pwBrush.Reset();
}

void BEDXOverlay::CreateDeviceResources(BEDirectXDevice& dx)
{
	HRESULT hr;

	// create the d2d device linked to the d3d device
	wrl::ComPtr<IDXGIDevice3> pDxgiDevice = nullptr;
	hr = dx.pDevice.As(&pDxgiDevice);
	BEDXRESOURCE_ERRORCHECK(hr);

	hr = pFactory->CreateDevice(pDxgiDevice.Get(), &pDevice);

	BEDXRESOURCE_ERRORCHECK(hr);

	// create the d2d device context

	hr = pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pContext);

	BEDXRESOURCE_ERRORCHECK(hr);

	// get the back buffer as a dxgi surface

	Microsoft::WRL::ComPtr<IDXGISurface2> pDxgiSurface = nullptr;
	hr = dx.pBackBuffer.As(&pDxgiSurface);

	BEDXRESOURCE_ERRORCHECK(hr);

	// create the bmp for the context, linked to the back buffer

	D2D1_BITMAP_PROPERTIES1 bmProp = {};
	bmProp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bmProp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bmProp.dpiX = dx.GetDPI();
	bmProp.dpiY = bmProp.dpiX;
	bmProp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	//bmProp.colorContext;

	hr = pContext->CreateBitmapFromDxgiSurface(pDxgiSurface.Get(), bmProp, &pBitmap);

	BEDXRESOURCE_ERRORCHECK(hr);

	// the bmp as the target

	pContext->SetTarget(pBitmap.Get());

	D2D1_SIZE_F size = pBitmap->GetSize();
	d2rect.right = size.width;
	d2rect.top = size.height;

	// create some brushes

	hr = pContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pwBrush);

	BEDXRESOURCE_ERRORCHECK(hr);

}

int BEDXOverlay::InitialiseTextWrite()
{
	HRESULT hr;

	// create the text writer

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &pWriteFactory);

	BEDXRESOURCE_ERRORCHECK(hr);

	hr = pWriteFactory->CreateTextFormat(
		fontName.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		locale.c_str(),
		&pWriteFormat);

	BEDXRESOURCE_ERRORCHECK(hr);

	pWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	BEDXRESOURCE_ERRORCHECK(hr);

	pWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	BEDXRESOURCE_ERRORCHECK(hr);

	return hr;
}

void BEDXOverlay::Draw()
{
	HRESULT hr;

	pContext->BeginDraw();

	std::wstring str = message.str();
	pContext->DrawTextW(str.c_str(), (UINT32)str.size(),
		pWriteFormat.Get(),
		d2rect,
		pwBrush.Get());

	hr = pContext->EndDraw();
	BEDXRESOURCE_ERRORCHECK(hr);

	ClearMessage();
}

void BEDXOverlay::ShowBitmap(BECanvas& canvas)
{
	HRESULT hr;

	//canvas.BufferToBMP(); // already done in this test case

	// to do: store this as properties
	D2D1_SIZE_F sizef = pBitmap->GetSize();
	D2D1_RECT_U rectu = { 0, 0, (UINT32)sizef.width, (UINT32)sizef.height };

	pContext->BeginDraw();

	hr = pBitmap->CopyFromMemory(&rectu, canvas.bmpSurface->GetData(), canvas.bmpSurface->GetPitchBytes());

	BEDXRESOURCE_ERRORCHECK(hr);

	hr = pContext->EndDraw();

	BEDXRESOURCE_ERRORCHECK(hr);
}
