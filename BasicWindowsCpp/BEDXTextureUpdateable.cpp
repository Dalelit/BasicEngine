#include "BEDXTextureUpdateable.h"

BEDXTextureUpdateable::BEDXTextureUpdateable(BEDirectXDevice& device, unsigned int width, unsigned int height, unsigned int pitch, void* source, DXGI_FORMAT format)
{
	HRESULT hr;

	sourceBuffer = source;
	sourcePitch = pitch;

	D3D11_TEXTURE2D_DESC1 texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texDesc.TextureLayout = D3D11_TEXTURE_LAYOUT_UNDEFINED;

	hr = device.pDevice->CreateTexture2D1(&texDesc, nullptr, &pTexture);

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

void BEDXTextureUpdateable::UpdateFromSource(BEDirectXDevice& device)
{
	device.pImmediateContext->UpdateSubresource(pTexture.Get(), 0u, nullptr, sourceBuffer, sourcePitch, 0u);
}
