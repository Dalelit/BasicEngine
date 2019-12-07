#include "BEDirectXDevice.h"

int BEDirectXDevice::Initialise(HWND hwnd, unsigned int width, unsigned int height)
{
	HRESULT hr;

	dpi = (float)GetDpiForWindow(hwnd);

	/////////////////// create device and back buffer

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL?
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL featureLevel; // will get populated with the actual feature level used... wanting 11_1

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG |  D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pDevice,
		&featureLevel,
		&pImmediateContext
	);

	if (FAILED(hr)) return hr;

	hr = pSwapChain.As(&pSwapChain1);

	if (FAILED(hr)) return hr;

	hr = pSwapChain1->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

	if (FAILED(hr)) return hr;

	hr = pDevice.As(&pDevice1);

	if (FAILED(hr)) return hr;

	hr = pImmediateContext.As(&pImmediateContext1);

	if (FAILED(hr)) return hr;

	hr = pDevice1->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView);

	if (FAILED(hr)) return hr;

	/////////////////// Rasterizer stage
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	pImmediateContext1->RSSetViewports(1, &viewport);

	/////////////////// Depth buffer

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;

	hr = pDevice1->CreateDepthStencilState(&dsDesc, &pDepthStencilStateOn);

	if (FAILED(hr)) return hr;

	dsDesc.DepthEnable = FALSE;

	hr = pDevice1->CreateDepthStencilState(&dsDesc, &pDepthStencilStateOff);

	if (FAILED(hr)) return hr;

	pImmediateContext1->OMSetDepthStencilState(pDepthStencilStateOn.Get(), 0u);

	D3D11_TEXTURE2D_DESC dtDesc = {};
	dtDesc.Width = width;
	dtDesc.Height = height;
	dtDesc.MipLevels = 1u;
	dtDesc.ArraySize = 1u;
	dtDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dtDesc.SampleDesc.Count = 1u;
	dtDesc.SampleDesc.Quality = 0u;
	dtDesc.Usage = D3D11_USAGE_DEFAULT;
	dtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dtDesc.CPUAccessFlags = 0u;
	dtDesc.MiscFlags = 0u;

	hr = pDevice1->CreateTexture2D(&dtDesc, nullptr, &pDepthTexture);

	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0u;

	hr = pDevice1->CreateDepthStencilView(pDepthTexture.Get(), &dsvDesc, &pDepthStencilView);

	if (FAILED(hr)) return hr;

	return 0;
}

void BEDirectXDevice::BeginFrame()
{
	pImmediateContext1->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	pImmediateContext1->ClearRenderTargetView(pRenderTargetView.Get(), clearColor);
	pImmediateContext1->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void BEDirectXDevice::PresentFrame()
{
	//pSwapChain->Present(1u, 0u);

	DXGI_PRESENT_PARAMETERS presentParams = {};
	pSwapChain1->Present1(1u, 0u, &presentParams);
}

void BEDirectXDevice::TurnOnDepthBuffer()
{
	pImmediateContext->OMSetDepthStencilState(pDepthStencilStateOn.Get(), 0u);
}

void BEDirectXDevice::TurnOffDepthBuffer()
{
	pImmediateContext->OMSetDepthStencilState(pDepthStencilStateOff.Get(), 0u);
}
