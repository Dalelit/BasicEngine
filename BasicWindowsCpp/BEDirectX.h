#pragma once
#include <d3d11.h>
#include "BEWorld.h"
#include "BECamera.h"
#include <wrl.h>

namespace wrl = Microsoft::WRL;

class BEDirectX
{
public:

	struct BEVertex {
		XMVECTOR position;
		XMVECTOR color;
	};

	BEDirectX();
	~BEDirectX();

	// To do: sort out the input parameters nicely
	int Initialise(HWND hwnd, unsigned int width, unsigned int height);

	int UpdateScene(BECamera* pCamera);

	int LoadScene(BEWorld* pWorld);

	int DoFrame();

	wrl::ComPtr<IDXGISwapChain> pSwapChain = NULL;
	wrl::ComPtr<ID3D11Device> pDevice = NULL;
	wrl::ComPtr<ID3D11DeviceContext> pImmediateContext = NULL;

	wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView = NULL;
	wrl::ComPtr<ID3D11Texture2D> pDepthTexture = NULL;

	wrl::ComPtr<ID3D11Resource> pBackBuffer = NULL;
	wrl::ComPtr<ID3D11RenderTargetView> pRenderTargetView = NULL;

	wrl::ComPtr<ID3D11Buffer> pTriangleBuffer = NULL;
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer = NULL;
	wrl::ComPtr<ID3D11VertexShader> pVertexShader = NULL;
	wrl::ComPtr<ID3D11PixelShader> pPixelShader = NULL;
	wrl::ComPtr<ID3D11InputLayout> pInputLayout = NULL;

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

private:
	BEVertex* verticies = NULL;
	unsigned int vertCount; // number of verticies to draw
};

