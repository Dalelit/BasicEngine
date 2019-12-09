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

	// create the d2d device linked to the d3d device
	hr = pFactory->CreateDevice(dx.pDxgiDevice.Get(), &pDevice);

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
	bmProp.dpiX = dx.dpi;
	bmProp.dpiY = dx.dpi;
	bmProp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	//bmProp.colorContext;

	hr = pContext->CreateBitmapFromDxgiSurface(pDxgiSurface.Get(), bmProp, &pBitmap);

	BEDXRESOURCE_ERRORCHECK(hr);

	// the bmp as the target

	pContext->SetTarget(pBitmap.Get());

	// create some brushes

	hr = pContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pwBrush);
	BEDXRESOURCE_ERRORCHECK(hr);
	hr = pContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::OrangeRed), &porBrush);
	BEDXRESOURCE_ERRORCHECK(hr);

	return hr;
}

void BEDXOverlay::Draw()
{
	HRESULT hr;

	D2D1_POINT_2F p0 = { 0, 0 };
	D2D1_POINT_2F p1 = { 300, 400 };
	D2D1_RECT_F d2rect = D2D1::RectF(0, 0, 400, 400);

	pContext->BeginDraw();

	//pContext->FillRectangle(d2rect, porBrush.Get());
	//pContext->DrawLine(p0, p1, pwBrush.Get());

	hr = pContext->EndDraw();
	BEDXRESOURCE_ERRORCHECK(hr);
}
