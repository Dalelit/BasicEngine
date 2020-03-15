#include "pch.h"
#include "BEDXVertexShader.h"
#include "BEUtil.h"


BEDXVertexShader::BEDXVertexShader(BEDirectXDevice& device, std::string src, BEDXVertexShaderLayout layout)
{
	CreateFromSource(device, src);
	SetLayout(device, *pBlob.Get(), layout);
}

void BEDXVertexShader::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	device.pImmediateContext->IASetInputLayout(pInputLayout.Get());
}

void BEDXVertexShader::CreateFromFile(BEDirectXDevice& device, std::string filename)
{
	HRESULT hr;

	hr = D3DReadFileToBlob(BEUtil::ToWString(filename).c_str(), &pBlob);

	BEDXRESOURCE_ERRORCHECK(hr);

	hr = device.pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);

	BEDXRESOURCE_ERRORCHECK(hr);
}

void BEDXVertexShader::CreateFromSource(BEDirectXDevice& device, std::string source)
{
	HRESULT hr;

	hr = D3DCompile(source.data(), source.length(), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &pBlob, NULL);

	BEDXRESOURCE_ERRORCHECK(hr);

	hr = device.pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);

	BEDXRESOURCE_ERRORCHECK(hr);
}

void BEDXVertexShader::SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob, BEDXVertexShaderLayout& layout)
{
	HRESULT hr;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc(layout.size());

	for (int i = 0; i < layout.size(); i++)
	{
		inputDesc[i].SemanticName = layout[i].name.c_str();
		inputDesc[i].SemanticIndex = 0;
		inputDesc[i].Format = layout[i].format;
		inputDesc[i].InputSlot = 0u;
		inputDesc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputDesc[i].InstanceDataStepRate = 0u;
	}

	hr = device.pDevice->CreateInputLayout(
		inputDesc.data(),
		(UINT)inputDesc.size(),
		shaderBlob.GetBufferPointer(),
		shaderBlob.GetBufferSize(),
		&pInputLayout);

	BEDXRESOURCE_ERRORCHECK(hr)
}
