#include "pch.h"
#include "BEDirectXDevice.h"

BEDirectXDevice::~BEDirectXDevice()
{
	if (IsFullScreen()) MakeWindowed(); // to do: confirm this is the best way to remove live objects at exit.
}

int BEDirectXDevice::Initialise(HWND hwnd)
{
	this->hwnd = hwnd;
	HRESULT hr;

	// for the create call, then get stored as later versions
	wrl::ComPtr<IDXGISwapChain> pSwapChain0 = nullptr;
	wrl::ComPtr<ID3D11Device> pDevice0 = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> pImmediateContext0 = nullptr;

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
	swapChainDesc.BufferCount = 2u;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL?
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL featureLevel; // will get populated with the actual feature level used... wanting 11_1

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG |  D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain0,
		&pDevice0,
		&featureLevel,
		&pImmediateContext0
	);
	if (FAILED(hr)) return hr;

	// store them as later versions of the interface
	hr = pSwapChain0.As(&pSwapChain);               if (FAILED(hr)) return hr;
	hr = pDevice0.As(&pDevice);                     if (FAILED(hr)) return hr;
	hr = pImmediateContext0.As(&pImmediateContext); if (FAILED(hr)) return hr;

	hr = ConfigurePipeline();
	if (FAILED(hr)) return hr;

	return 0;
}

int BEDirectXDevice::ConfigurePipeline()
{
	HRESULT hr;

	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);

	if (FAILED(hr)) return hr;

	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView);

	if (FAILED(hr)) return hr;

	pBackBuffer->GetDesc(&bufferDesc);

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)bufferDesc.Width;
	viewport.Height = (float)bufferDesc.Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	pImmediateContext->RSSetViewports(1, &viewport);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;

	hr = pDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilStateOn);

	if (FAILED(hr)) return hr;

	pImmediateContext->OMSetDepthStencilState(pDepthStencilStateOn.Get(), 0u);

	D3D11_TEXTURE2D_DESC dtDesc = {};
	dtDesc.Width = bufferDesc.Width;
	dtDesc.Height = bufferDesc.Height;
	dtDesc.MipLevels = 1u;
	dtDesc.ArraySize = 1u;
	dtDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dtDesc.SampleDesc.Count = 1u;
	dtDesc.SampleDesc.Quality = 0u;
	dtDesc.Usage = D3D11_USAGE_DEFAULT;
	dtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dtDesc.CPUAccessFlags = 0u;
	dtDesc.MiscFlags = 0u;

	hr = pDevice->CreateTexture2D(&dtDesc, nullptr, &pDepthTexture);

	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = dtDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0u;

	hr = pDevice->CreateDepthStencilView(pDepthTexture.Get(), &dsvDesc, &pDepthStencilView);

	if (FAILED(hr)) return hr;

	return 0;
}

void BEDirectXDevice::ReleasePipeline()
{
	pImmediateContext->OMSetRenderTargets(0, 0, 0);

	pRenderTargetView.Reset();
	pBackBuffer.Reset();
	pDepthStencilStateOn.Reset();
	pDepthTexture.Reset();
	pDepthStencilView.Reset();

	pImmediateContext->ClearState();
	pImmediateContext->Flush();
}

void BEDirectXDevice::BeginFrame()
{
	pImmediateContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), clearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void BEDirectXDevice::PresentFrame()
{
	pSwapChain->Present(1u, 0u);

	//DXGI_PRESENT_PARAMETERS presentParams = {};
	//pSwapChain->Present1(1u, 0u, &presentParams);
}

bool BEDirectXDevice::IsFullScreen()
{
	BOOL fullScreen;
	HRESULT hr = pSwapChain->GetFullscreenState(&fullScreen, nullptr);
	BEDX_CHECKHR_LOGERROR(hr, "Error with GetFullscreenState");

	return fullScreen;
}

void BEDirectXDevice::MakeFullScreen()
{
	// to do: check if the request is the current state and then do nothing?
	ChangeScreenSize(true);
}

void BEDirectXDevice::MakeWindowed()
{
	// to do: check if the request is the current state and then do nothing?
	ChangeScreenSize(false);
}

void BEDirectXDevice::ChangeScreenSize(bool fullscreen)
{
	HRESULT hr = pSwapChain->SetFullscreenState(fullscreen, nullptr);
	BEDX_CHECKHR_THROW(hr, "Error with SetFullscreenState");

	ReleasePipeline();

	hr = pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0); // resize, retaining previous settings
	BEDX_CHECKHR_THROW(hr, "ResizeBuffers failed");

	hr = ConfigurePipeline();
	BEDX_CHECKHR_THROW(hr, "CreatePipeline failed");
}

void BEDirectXDevice::LogDebugInfo()
{
	BELOG_FUNC_SCOPE;

	HRESULT hr;

	BELOG_DEBUG("D3D11_TEXTURE2D_DESC bufferDesc");
	BELOG_DEBUG(" Format " + std::to_string(bufferDesc.Format));
	BELOG_DEBUG(" Width " + std::to_string(bufferDesc.Width));
	BELOG_DEBUG(" Height " + std::to_string(bufferDesc.Height));
	
	wrl::ComPtr<IDXGIDevice3> pDxgiDevice;
	pDevice.As(&pDxgiDevice);

	wrl::ComPtr<IDXGIAdapter> pAdapter;
	pDxgiDevice->GetAdapter(&pAdapter);

	DXGI_ADAPTER_DESC desc;
	pAdapter->GetDesc(&desc);

	BELOG_DEBUG("DXGI_ADAPTER_DESC");
	BELOG_DEBUG(L" Description " + std::wstring(desc.Description));
	BELOG_DEBUG(" Revision " + std::to_string(desc.Revision));

	wrl::ComPtr<ID3D11Debug> pDebug = nullptr;
	hr = pDevice->QueryInterface(__uuidof(ID3D11Debug), &pDebug);
	if (FAILED(hr)) BELOG_ERROR("Failed to get ID3D11Debug");

	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
	//pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}
