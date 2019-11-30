#pragma once

#include <d3d11.h>
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
		
	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

	wrl::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	wrl::ComPtr<ID3D11Device> pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> pImmediateContext = nullptr;
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;
	wrl::ComPtr<ID3D11Texture2D> pDepthTexture = nullptr;
};

