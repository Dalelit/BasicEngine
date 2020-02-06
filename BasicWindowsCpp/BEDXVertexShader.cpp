#include "BEDXVertexShader.h"
#include <d3dcompiler.h>


BEDXVertexShader::BEDXVertexShader(BEDirectXDevice& device, std::wstring filename)
	:
	filename(filename)
{
}

void BEDXVertexShader::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	device.pImmediateContext->IASetInputLayout(pInputLayout.Get());
}

void BEDXVertexShader::Initialise(BEDirectXDevice& device)
{
	HRESULT hr;

	wrl::ComPtr<ID3DBlob> pBlob = nullptr;
	hr = D3DReadFileToBlob(filename.c_str(), &pBlob);

	BEDXRESOURCE_ERRORCHECK(hr);

	hr = device.pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);

	BEDXRESOURCE_ERRORCHECK(hr);

	SetLayout(device, *pBlob.Get());
}

void BEDXVertexShaderPosNorColTex::SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob)
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC inputDesc[4] = {}; // reminder: update CreateIntputLayout number

	inputDesc[0].SemanticName = "Position";
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputDesc[0].InputSlot = 0u;
	inputDesc[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0u;

	inputDesc[1].SemanticName = "Normal";
	inputDesc[1].SemanticIndex = 0;
	inputDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputDesc[1].InputSlot = 0u;
	inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[1].InstanceDataStepRate = 0u;

	inputDesc[2].SemanticName = "Color";
	inputDesc[2].SemanticIndex = 0;
	inputDesc[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputDesc[2].InputSlot = 0u;
	inputDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[2].InstanceDataStepRate = 0u;

	inputDesc[3].SemanticName = "Texcoord";
	inputDesc[3].SemanticIndex = 0;
	inputDesc[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDesc[3].InputSlot = 0u;
	inputDesc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[3].InstanceDataStepRate = 0u;

	hr = device.pDevice->CreateInputLayout(
		inputDesc,
		ARRAYSIZE(inputDesc),
		shaderBlob.GetBufferPointer(),
		shaderBlob.GetBufferSize(),
		&pInputLayout);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXVertexShaderPosTex::SetLayout(BEDirectXDevice& device, ID3DBlob& shaderBlob)
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC inputDesc[2] = {}; // reminder: update CreateIntputLayout number

	inputDesc[0].SemanticName = "Position";
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[0].InputSlot = 0u;
	inputDesc[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0u;

	inputDesc[1].SemanticName = "Texcoord";
	inputDesc[1].SemanticIndex = 0;
	inputDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDesc[1].InputSlot = 0u;
	inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[1].InstanceDataStepRate = 0u;

	hr = device.pDevice->CreateInputLayout(
		inputDesc,
		ARRAYSIZE(inputDesc),
		shaderBlob.GetBufferPointer(),
		shaderBlob.GetBufferSize(),
		&pInputLayout);

	BEDXRESOURCE_ERRORCHECK(hr)
}
