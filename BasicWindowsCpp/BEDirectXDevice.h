#pragma once

#include "pch.h"
#include "BELogger.h"

// To do: Proper exceptions
#define BEDXRESOURCE_ERRORCHECK(hr) if (FAILED(hr)) throw hr;
#define BEDX_CHECKHR_THROW(hr, msg) if (FAILED(hr)) throw msg;
#define BEDX_CHECKHR_LOGERROR(hr, msg) BELOG_ERROR_IF(!SUCCEEDED(hr), msg)

namespace wrl = Microsoft::WRL;

class BEDirectXDevice
{
public:
	~BEDirectXDevice();
	int Initialise(HWND hwnd);
	void BeginFrame();
	void PresentFrame();

	bool IsFullScreen();
	void MakeFullScreen();
	void MakeWindowed();
	void Resize();

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

	wrl::ComPtr<IDXGISwapChain1> pSwapChain = nullptr;
	wrl::ComPtr<ID3D11Device3> pDevice = nullptr;
	wrl::ComPtr<ID3D11DeviceContext3> pImmediateContext = nullptr;
	wrl::ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;
	wrl::ComPtr<ID3D11Texture2D> pDepthTexture = nullptr;
	wrl::ComPtr<ID3D11DepthStencilState> pDepthStencilStateOn = nullptr;

	inline float GetDPI() { return (float)GetDpiForWindow(hwnd); }

	inline unsigned int GetWidth() { return bufferDesc.Width; }
	inline unsigned int GetHeight() { return bufferDesc.Height; }
	inline float GetAspectRatio() { return (float)GetWidth() / (float)GetHeight(); }

	void LogDebugInfo();

protected:
	HWND hwnd;
	D3D11_TEXTURE2D_DESC bufferDesc;

	int ConfigurePipeline();
	void ReleasePipeline();
	void ChangeScreenSize(bool fullscreen);
};

