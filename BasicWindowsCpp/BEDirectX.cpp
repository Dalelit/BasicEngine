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
	if (pVertexShaderBlob) pVertexShaderBlob->Release();
	if (pPixelShaderBlob) pPixelShaderBlob->Release();
	if (pVertexShader) pVertexShader->Release();
	if (pPixelShader) pPixelShader->Release();
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

int BEDirectX::LoadScene(BEWorld* _pWorld, BECamera* _pCamera, unsigned int width, unsigned int height)
{
	HRESULT hr;

	/////////////////// Input Assembler stage
	// In draw

	/////////////////// Vertex Shader stage
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pVertexShaderBlob);
	if (FAILED(hr)) return hr;

	hr = pDevice->CreateVertexShader(
		pVertexShaderBlob->GetBufferPointer(),
		pVertexShaderBlob->GetBufferSize(),
		nullptr, &pVertexShader);

	if (FAILED(hr)) return hr;

	pImmediateContext->VSSetShader(pVertexShader, nullptr, 0u);

	D3D11_INPUT_ELEMENT_DESC inputDesc[1] = {};
	inputDesc[0].SemanticName = "Position";
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[0].InputSlot = 0u;
	inputDesc[0].AlignedByteOffset = 0u;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0u;

	hr = pDevice->CreateInputLayout(
		inputDesc, 1u,
		pVertexShaderBlob->GetBufferPointer(),
		pVertexShaderBlob->GetBufferSize(),
		&pInputLayout);

	if (FAILED(hr)) return hr;

	pImmediateContext->IASetInputLayout(pInputLayout);

	/////////////////// Rasterizer stage
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	pImmediateContext->RSSetViewports(1, &viewport);

	/////////////////// Pixel Shader stage
	hr = D3DReadFileToBlob(L"PixelShader.cso", &pPixelShaderBlob);
	if (FAILED(hr)) return hr;

	hr = pDevice->CreatePixelShader(
		pPixelShaderBlob->GetBufferPointer(),
		pPixelShaderBlob->GetBufferSize(),
		nullptr, &pPixelShader);

	if (FAILED(hr)) return hr;

	pImmediateContext->PSSetShader(pPixelShader, nullptr, 0u);

	/////////////////// Output merger stage
	pImmediateContext->OMSetRenderTargets(1u, &pRenderTargetView, nullptr);

	return 0;
}

int BEDirectX::DoFrame()
{
	if (color[2] > 1.0f) color[2] = 0.0f;

	pImmediateContext->ClearRenderTargetView(pRenderTargetView, color);


	/////////////////// Input Assembler stage

	struct Vertex {
		float x, y, z;
	};

	Vertex verts[] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.5f, 0.0f},
		{0.5f, 0.0f, 0.0f},
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

	if (pTriangleBuffer) pTriangleBuffer->Release();
	HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &triangleData, &pTriangleBuffer);
	if (FAILED(hr)) return hr;

	// set the triangle vertex buffer
	UINT bufferStrides[] = { sizeof(Vertex) };
	UINT bufferOffsets[] = { 0 };
	pImmediateContext->IASetVertexBuffers(0, 1, &pTriangleBuffer, bufferStrides, bufferOffsets);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/////////////////// Draw
	unsigned int vertCount = (UINT)std::size(verts);
	pImmediateContext->Draw(vertCount, 0u);
	pSwapChain->Present(1u, 0u);


	color[2] += 0.01f;

	return 0;
}
