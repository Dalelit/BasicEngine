#include "pch.h"
#include "BEDXTexture.h"

using namespace DirectX;

BEDXTexture::BEDXTexture(BEDirectXDevice& device, BESurface2D<XMVECTOR>& surface, DXGI_FORMAT format)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC1 texDesc = {};
	texDesc.Width = surface.GetWidth();
	texDesc.Height = surface.GetHeight();
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//texDesc.CPUAccessFlags;
	//texDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = surface.GetData();
	data.SysMemPitch = surface.GetPitchBytes();

	hr = device.pDevice->CreateTexture2D1(&texDesc, &data, &pTexture);

	BEDXRESOURCE_ERRORCHECK(hr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = device.pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pSRView);

	BEDXRESOURCE_ERRORCHECK(hr);

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // To do: check what the best one is
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	hr = device.pDevice->CreateSamplerState(&sampDesc, &pSamplerState);

	BEDXRESOURCE_ERRORCHECK(hr);
}

BEDXTexture::BEDXTexture(BEDirectXDevice& device, BETexture& texture, DXGI_FORMAT format)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC1 texDesc = {};
	texDesc.Width = texture.GetWidth();
	texDesc.Height = texture.GetHeight();
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//texDesc.CPUAccessFlags;
	//texDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = texture.surface->GetData();
	data.SysMemPitch = texture.surface->GetPitchBytes();

	hr = device.pDevice->CreateTexture2D1(&texDesc, &data, &pTexture);

	BEDXRESOURCE_ERRORCHECK(hr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = device.pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pSRView);

	BEDXRESOURCE_ERRORCHECK(hr);

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // To do: check what the best one is
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	hr = device.pDevice->CreateSamplerState(&sampDesc, &pSamplerState);

	BEDXRESOURCE_ERRORCHECK(hr);
}

void BEDXTexture::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->PSSetShaderResources(slot, 1u, pSRView.GetAddressOf());
	device.pImmediateContext->PSSetSamplers(slot, 1u, pSamplerState.GetAddressOf());
}
