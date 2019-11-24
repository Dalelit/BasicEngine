#pragma once
#include <d3d11.h>
#include "BEWorld.h"
#include "BECamera.h"

// To Do
// - use ComPtr

class BEDirectX
{
public:

	struct BEVertex {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	BEDirectX();
	~BEDirectX();

	// To do: sort out the input parameters nicely
	int Initialise(HWND hwnd, unsigned int width, unsigned int height);

	int UpdateScene(BECamera* pCamera);

	int LoadScene(BEWorld* pWorld);

	int DoFrame();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	IDXGISwapChain* pSwapChain = NULL;
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pImmediateContext = NULL;

	ID3D11DepthStencilState* pDepthStencilState = NULL; // to do - don't need to keep this
	ID3D11DepthStencilView* pDepthStencilView = NULL;
	ID3D11Texture2D* pDepthTexture = NULL;

	ID3D11Resource* pBackBuffer = NULL;
	ID3D11RenderTargetView* pRenderTargetView = NULL;

	ID3D11Buffer* pTriangleBuffer = NULL;
	ID3D11Buffer* pConstantBuffer = NULL;
	ID3D11VertexShader* pVertexShader = NULL;
	ID3D11PixelShader* pPixelShader = NULL;
	ID3D11InputLayout* pInputLayout = NULL;

	ID3DBlob* pVertexShaderBlob = NULL;
	ID3DBlob* pPixelShaderBlob = NULL;

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

private:
	BEVertex* verticies = NULL;
	unsigned int vertCount; // number of verticies to draw
};

