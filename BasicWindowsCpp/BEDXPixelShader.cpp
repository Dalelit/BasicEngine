#include "BEDXPixelShader.h"
#include <d3dcompiler.h>

BEDXPixelShader::BEDXPixelShader(BEDirectXDevice& device, std::wstring filename)
	:
	filename(filename)
{
	HRESULT hr;

	wrl::ComPtr<ID3DBlob> pBlob = nullptr;
	hr = D3DReadFileToBlob(filename.c_str(), &pBlob);

	BEDXRESOURCE_ERRORCHECK(hr)

	hr = device.pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXPixelShader::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
