#include "BEDXOverlay.h"


#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

int BEDXOverlay::Initialise(BEDirectXDevice& dx)
{
	HRESULT hr;

	// create the factory
	Microsoft::WRL::ComPtr<ID2D1Factory1> pFactory = nullptr;

	D2D1_FACTORY_OPTIONS factoryOptions = {};
	factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_WARNING;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1),
		&factoryOptions,
		(void**)&pFactory);

	BEDXRESOURCE_ERRORCHECK(hr);

	// create a device, linked to d3d device

	// get the d3d device
	Microsoft::WRL::ComPtr<IDXGIDevice1> pdxgiDevice = nullptr;
	hr = dx.pDevice1.As(&pdxgiDevice);

	BEDXRESOURCE_ERRORCHECK(hr);

	// create the d2d device linked to the d3d device
	Microsoft::WRL::ComPtr<ID2D1Device> pDevice = nullptr;
	hr = pFactory->CreateDevice(pdxgiDevice.Get(), &pDevice);

	BEDXRESOURCE_ERRORCHECK(hr);

	// create the d2d device context

	hr = pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pContext);

	BEDXRESOURCE_ERRORCHECK(hr);

	// get the back buffer as a dxgi surface

	Microsoft::WRL::ComPtr<IDXGISurface1> pDxgiSurface = nullptr;
	//hr = dx.pBackBuffer.As(&pDxgiSurface);
	hr = dx.pSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pDxgiSurface));

	BEDXRESOURCE_ERRORCHECK(hr);

	// create the bmp for the context, linked to the back buffer

	D2D1_BITMAP_PROPERTIES1 bmProp = {};
	bmProp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bmProp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bmProp.dpiX = dx.dpi;
	bmProp.dpiY = dx.dpi;
	bmProp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	//bmProp.colorContext;

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> pBitmap = nullptr;
	hr = pContext->CreateBitmapFromDxgiSurface(pDxgiSurface.Get(), bmProp, &pBitmap);

	BEDXRESOURCE_ERRORCHECK(hr);

	// the bmp as the target

	pContext->SetTarget(pBitmap.Get());
	pContext->SetDpi(dx.dpi, dx.dpi);

	// create some brushes

	hr = pContext->CreateSolidColorBrush(whiteSolid, &pwBrush);
	BEDXRESOURCE_ERRORCHECK(hr);
	hr = pContext->CreateSolidColorBrush(oredSolid, &porBrush);
	BEDXRESOURCE_ERRORCHECK(hr);

	return hr;

	////////////// Old way...?
	//D2D1_RENDER_TARGET_PROPERTIES rtProps = {};
	//rtProps.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	//rtProps.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM; // DXGI_FORMAT_UNKNOWN;
	//rtProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	//rtProps.dpiX = (float)dx.dpi;
	//rtProps.dpiY = (float)dx.dpi;
	//rtProps.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	//hr = pFactory->CreateDxgiSurfaceRenderTarget(pDxgiSurface.Get(), &rtProps, &pRenderTgt);
	//BEDXRESOURCE_ERRORCHECK(hr);
	//hr = pRenderTgt->CreateSolidColorBrush(whiteSolid, &pwBrush);
	//BEDXRESOURCE_ERRORCHECK(hr);
	//hr = pRenderTgt->CreateSolidColorBrush(oredSolid, &porBrush);
	//BEDXRESOURCE_ERRORCHECK(hr);
	//////////////

}

void BEDXOverlay::Draw()
{
	HRESULT hr;

	D2D1_POINT_2F p0 = { 0, 0 };
	D2D1_POINT_2F p1 = { 300, 400 };
	D2D1_RECT_F d2rect = D2D1::RectF(0, 0, 400, 400);

	//pRenderTgt->BeginDraw();
	//pRenderTgt->SetTransform(D2D1::Matrix3x2F::Identity());
	//pRenderTgt->DrawLine(p0, p1, pwBrush.Get(), 3.0f);
	//pRenderTgt->FillRectangle(d2rect, porBrush.Get());
	//hr = pRenderTgt->EndDraw();

	pContext->BeginDraw();
	pContext->SetTransform(D2D1::Matrix3x2F::Identity());
	pContext->FillRectangle(d2rect, porBrush.Get());
	pContext->DrawLine(p0, p1, pwBrush.Get(), 3.0f);
	hr = pContext->EndDraw();
	BEDXRESOURCE_ERRORCHECK(hr);
}
