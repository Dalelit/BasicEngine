#pragma once

#include <d3d11_3.h>
#include <wrl.h>

// To do: Proper exceptions
#define BEDXRESOURCE_ERRORCHECK(hr) if (FAILED(hr)) throw hr;

namespace wrl = Microsoft::WRL;

class BEDirectXDevice
{
public:
	~BEDirectXDevice() {};
	int Initialise(HWND hwnd, unsigned int width, unsigned int height);
	void BeginFrame();
	void PresentFrame();

	void TurnOnDepthBuffer();
	void TurnOffDepthBuffer();

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

	wrl::ComPtr<IDXGISwapChain1> pSwapChain = nullptr;
	wrl::ComPtr<ID3D11Device3> pDevice = nullptr;
	wrl::ComPtr<IDXGIDevice3> pDxgiDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext3> pImmediateContext = nullptr;
	wrl::ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;
	wrl::ComPtr<ID3D11Texture2D> pDepthTexture = nullptr;
	wrl::ComPtr<ID3D11DepthStencilState> pDepthStencilStateOn = nullptr;
	wrl::ComPtr<ID3D11DepthStencilState> pDepthStencilStateOff = nullptr;

	float dpi = 0;
};

