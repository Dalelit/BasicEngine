#include "BEDirectX.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

BEDirectX::BEDirectX()
{
}

BEDirectX::~BEDirectX()
{
	if (pSwapChain) pSwapChain->Release();
	if (pDevice) pDevice->Release();
	if (pImmediateContext) pImmediateContext->Release();
	if (pBackBuffer) pBackBuffer->Release();
	if (pRenderTargetView) pRenderTargetView->Release();
	if (pTriangleBuffer) pTriangleBuffer->Release();
}

int BEDirectX::Initialise(HWND hwnd)
{
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
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT hresult = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pDevice,
		NULL,
		&pImmediateContext
	);

	if (FAILED(hresult)) return hresult;

	hresult = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&pBackBuffer);
		
	if (FAILED(hresult)) return hresult;

	hresult = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	if (FAILED(hresult)) return hresult;

	return hresult;
}

int BEDirectX::LoadScene(BEWorld* _pWorld, BECamera* _pCamera)
{
	HRESULT hr;

	/////////////////// Input Assembler stage

	struct Vertex {
		float x, y, z;
	};

	Vertex verts[] = {
		{0.0f, 0.0f, 0.0f},
		{0.5f, 0.0f, 0.0f},
		{0.0f, 0.5f, 0.0f},
	};

	// create triangle data
	D3D11_SUBRESOURCE_DATA triangleData = {};
	triangleData.SysMemPitch = 0;
	triangleData.SysMemSlicePitch = 0;
	triangleData.pSysMem = verts;

	// create the triangle buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(verts);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(Vertex);

	hr = pDevice->CreateBuffer(&bufferDesc, &triangleData, &pTriangleBuffer);
	if (FAILED(hr)) return hr;

	// set the triangle vertex buffer
	UINT bufferStrides[] = { sizeof(verts) };
	UINT bufferOffsets[] = { 0 };
	pImmediateContext->IASetVertexBuffers(0, 1, &pTriangleBuffer, bufferStrides, bufferOffsets);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/////////////////// Vertex Shader stage
	ID3DBlob* pBlob = nullptr;
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	if (FAILED(hr)) return hr;


	if (pBlob) pBlob->Release();

	/////////////////// Rasterizer stage

	//pImmediateContext->RSSetViewports

	/////////////////// Pixel Shader stage
	//pImmediateContext->PSSetShader(nullptr, nullptr, 0);

	/////////////////// Output merger stage

	//pImmediateContext->OMSetRenderTargets


	// draw
	pImmediateContext->Draw((UINT)std::size(verts), 0u);

	return 0;
}

int BEDirectX::DoFrame()
{
	if (color[2] > 1.0f) color[2] = 0.0f;

	pImmediateContext->ClearRenderTargetView(pRenderTargetView, color);

	color[2] += 0.01f;

	pSwapChain->Present(1u, 0u);

	return 0;
}
