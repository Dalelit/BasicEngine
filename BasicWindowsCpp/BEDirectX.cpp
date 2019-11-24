#include "BEDirectX.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define BEDIRECTX_VERT_BUFFER_MAXSIZE 1000

BEDirectX::BEDirectX()
{
	verticies = new BEVertex[BEDIRECTX_VERT_BUFFER_MAXSIZE];
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
	if (pConstantBuffer) pConstantBuffer->Release();
	if (pDepthStencilState) pDepthStencilState->Release();
	if (pDepthStencilView) pDepthStencilView->Release();
	if (pDepthTexture) pDepthTexture->Release();

	if (verticies) delete verticies;
}

int BEDirectX::Initialise(HWND hwnd, unsigned int width, unsigned int height)
{
	HRESULT hr;

	/////////////////// create device and back buffer

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
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(
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

	if (FAILED(hr)) return hr;

	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&pBackBuffer);
		
	if (FAILED(hr)) return hr;

	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	if (FAILED(hr)) return hr;

	/////////////////// Depth buffer

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = pDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilState);

	if (FAILED(hr)) return hr;

	pImmediateContext->OMSetDepthStencilState(pDepthStencilState, 1u);

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

	hr = pDevice->CreateTexture2D(&dtDesc, nullptr, &pDepthTexture);

	if (FAILED(hr)) return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0u;

	hr = pDevice->CreateDepthStencilView(pDepthTexture, &dsvDesc, &pDepthStencilView);

	if (FAILED(hr)) return hr;

	// happens in the draw loop
	//pImmediateContext->OMSetRenderTargets(1u, &pRenderTargetView, pDepthStencilView);


	/////////////////// Vertex Shader stage
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pVertexShaderBlob);
	if (FAILED(hr)) return hr;

	hr = pDevice->CreateVertexShader(
		pVertexShaderBlob->GetBufferPointer(),
		pVertexShaderBlob->GetBufferSize(),
		nullptr, &pVertexShader);

	if (FAILED(hr)) return hr;

	pImmediateContext->VSSetShader(pVertexShader, nullptr, 0u);

	D3D11_INPUT_ELEMENT_DESC inputDesc[2] = {};
	inputDesc[0].SemanticName = "Position";
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[0].InputSlot = 0u;
	inputDesc[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // 0u;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0u;
	inputDesc[1].SemanticName = "Color";
	inputDesc[1].SemanticIndex = 0;
	inputDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputDesc[1].InputSlot = 0u;
	inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[1].InstanceDataStepRate = 0u;

	hr = pDevice->CreateInputLayout(
		inputDesc, 2u,
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

	return hr;
}

int BEDirectX::LoadScene(BEWorld* pWorld)
{
	HRESULT hr;

	UINT vertIndx = 0;

	for (UINT eIndx = 0; eIndx < pWorld->entityCount; eIndx++)
	{
		BEMesh* m = pWorld->entities[eIndx]->mesh;

		if (m)
		{
			for (UINT i = 0; i < m->vCount; i++)
			{
				// To Do: efficient way to do this?
				verticies[vertIndx].position = m->verticies[i];
				verticies[vertIndx].color = pWorld->entities[eIndx]->color;
				vertIndx++;
			}
		}
	}
	vertCount = vertIndx;

	//verticies[0].position = { 0.0f, 0.0f, 0.0f };
	//verticies[1].position = { 0.0f, 0.5f, 0.0f };
	//verticies[2].position = { 0.5f, 0.0f, 0.0f };
	//vertCount = 3;

	// create triangle data
	D3D11_SUBRESOURCE_DATA triangleData = {};
	triangleData.SysMemPitch = 0;
	triangleData.SysMemSlicePitch = 0;
	triangleData.pSysMem = verticies;

	// create the triangle buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(BEVertex) * vertCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(BEVertex);

	if (pTriangleBuffer) pTriangleBuffer->Release();
	hr = pDevice->CreateBuffer(&bufferDesc, &triangleData, &pTriangleBuffer);
	if (FAILED(hr)) return hr;

	// set the triangle vertex buffer
	UINT bufferStrides[] = { sizeof(BEVertex) };
	UINT bufferOffsets[] = { 0 };
	pImmediateContext->IASetVertexBuffers(0, 1, &pTriangleBuffer, bufferStrides, bufferOffsets);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return hr;
}

int BEDirectX::UpdateScene(BECamera* pCamera)
{
	HRESULT hr = 0;

	struct ConstantBuffer {
		XMMATRIX transformation;
	};

	ConstantBuffer constBuff;
	constBuff.transformation = XMMatrixTranspose(pCamera->GetViewProjectionMatrix());

	D3D11_SUBRESOURCE_DATA constBufferData = {};
	constBufferData.SysMemPitch = 0;
	constBufferData.SysMemSlicePitch = 0;
	constBufferData.pSysMem = &constBuff;

	// create the buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(ConstantBuffer);

	if (pConstantBuffer) pConstantBuffer->Release();
	hr = pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);
	if (FAILED(hr)) return hr;

	// set the constant buffer
	pImmediateContext->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);

	return hr;
}

int BEDirectX::DoFrame()
{
	pImmediateContext->OMSetRenderTargets(1u, &pRenderTargetView, pDepthStencilView);

	pImmediateContext->ClearRenderTargetView(pRenderTargetView, clearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);

	pImmediateContext->Draw(vertCount, 0u);

	pSwapChain->Present(1u, 0u);


	return 0;
}
