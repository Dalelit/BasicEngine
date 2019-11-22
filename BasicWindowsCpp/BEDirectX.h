#pragma once
#include <d3d11.h>
#include "BEWorld.h"
#include "BECamera.h"

// To Do
// - use ComPtr

class BEDirectX
{
public:
	BEDirectX();
	~BEDirectX();
	int Initialise(HWND hwnd);

	int LoadScene(BEWorld* _pWorld, BECamera* _pCamera, unsigned int width, unsigned int height);

	int DoFrame();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	IDXGISwapChain* pSwapChain = NULL;
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pImmediateContext = NULL;

	ID3D11Resource* pBackBuffer = NULL;
	ID3D11RenderTargetView* pRenderTargetView = NULL;

	ID3D11Buffer* pTriangleBuffer = NULL;
	ID3D11VertexShader* pVertexShader = NULL;
	ID3D11PixelShader* pPixelShader = NULL;
	ID3D11InputLayout* pInputLayout = NULL;

	ID3DBlob* pVertexShaderBlob = NULL;
	ID3DBlob* pPixelShaderBlob = NULL;

	float color[4] = { 1.0f,0.0f,0.0f,1.0f };

};

