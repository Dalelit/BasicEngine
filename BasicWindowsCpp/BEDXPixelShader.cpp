#include "pch.h"
#include "BEDXPixelShader.h"
#include "BEUtil.h"

void BEDXPixelShader::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

BEDXPixelShaderFile::BEDXPixelShaderFile(BEDirectXDevice& device, std::string filename)	:
	filename(filename)
{
	HRESULT hr;

	hr = D3DReadFileToBlob(BEUtil::ToWString(filename).c_str(), &pBlob);

	BEDXRESOURCE_ERRORCHECK(hr)

	hr = device.pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader);

	BEDXRESOURCE_ERRORCHECK(hr)
}

BEDXPixelShaderSource::BEDXPixelShaderSource(BEDirectXDevice& device, std::string source)
{
	HRESULT hr;

	hr = D3DCompile(source.data(), source.length(), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pBlob, NULL);

	BEDXRESOURCE_ERRORCHECK(hr)

		hr = device.pDevice->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pPixelShader);

	BEDXRESOURCE_ERRORCHECK(hr)
}
