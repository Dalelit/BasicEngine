#include "BEDXShowCanvas.h"
#include "BEDXVertexBuffer.h"
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"

BEDXShowCanvas::BEDXShowCanvas()
{
	// set the mesh
	mesh[0] = { { -1,-1,0 }, {0,0} };
	mesh[1] = { { -1, 1,0 }, {0,1} };
	mesh[2] = { {  1, 1,0 }, {1,1} };

	mesh[3] = { { -1,-1,0 }, {0,0} };
	mesh[4] = { {  1, 1,0 }, {1,1} };
	mesh[5] = { {  1,-1,0 }, {1,0} };
}

void BEDXShowCanvas::Initialise(BEDirectX& dx)
{
	// create the vertex buffer
	BEDXVertexBuffer* pVB = new BEDXVertexBuffer(dx.device, &mesh, ARRAYSIZE(mesh), sizeof(BETexVertex)); // to do: hard coded vertex size?
	dx.drawables.push_back(pVB);

	// create the vertex shader
	BEDXVertexShader* pVS = new BEDXVertexShader(dx.device, L"VSPassthrough.cso", BEDXVertexShader::InputLayout::POSITION3_TEXCOORD2);
	pVS->Bind(dx.device);
	dx.resources.push_back(pVS);

	// create the pixel shader
	BEDXPixelShader* pPS = new BEDXPixelShader(dx.device, L"PSPassthrough.cso");
	pPS->Bind(dx.device);
	dx.resources.push_back(pPS);
}

void BEDXShowCanvas::InitialiseTexture(BEDirectX& dx, BECanvas& canvas)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC1 texDesc = {};
	texDesc.Width = canvas.width;
	texDesc.Height = canvas.height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = canvas.buffer;
	data.SysMemPitch = canvas.GetCanvasPitch();

	hr = dx.device.pDevice->CreateTexture2D1(&texDesc, &data, &pTexture);

	BEDXRESOURCE_ERRORCHECK(hr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = dx.device.pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pSRView);

	BEDXRESOURCE_ERRORCHECK(hr);

	dx.device.pImmediateContext->PSSetShaderResources(0u, 1u, pSRView.GetAddressOf());

	//dx.device.pImmediateContext->GenerateMips(pSRView.Get());

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // To do: check what the best one is
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	hr = dx.device.pDevice->CreateSamplerState(&sampDesc, &pSamplerState);
	
	BEDXRESOURCE_ERRORCHECK(hr);

	dx.device.pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
}

void BEDXShowCanvas::UpdateTexture(BEDirectX& dx, BECanvas& canvas)
{
	dx.device.pImmediateContext->UpdateSubresource(pTexture.Get(), 0u, nullptr, canvas.buffer, canvas.GetCanvasPitch(), 0u);
}

void BEDXShowCanvas::Draw(BEDirectX& dx, BECanvas& canvas)
{
	dx.device.TurnOffDepthBuffer();
	dx.device.BeginFrame();
	UpdateTexture(dx, canvas);
	for (auto d : dx.drawables) d->Draw(dx.device);
	dx.device.PresentFrame();
}
