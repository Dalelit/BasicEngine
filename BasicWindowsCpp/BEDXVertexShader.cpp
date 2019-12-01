#include "BEDXVertexShader.h"
#include <d3dcompiler.h>


BEDXVertexShader::BEDXVertexShader(BEDirectXDevice& device, LPCWSTR filename)
{
	HRESULT hr;

	wrl::ComPtr<ID3DBlob> pBlob = nullptr;
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);

	BEDXRESOURCE_ERRORCHECK(hr)

	hr = device.pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);

	BEDXRESOURCE_ERRORCHECK(hr)

	D3D11_INPUT_ELEMENT_DESC inputDesc[3] = {}; // reminder: update CreateIntputLayout number
	
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

	hr = device.pDevice->CreateInputLayout(
		inputDesc,
		3u,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXVertexShader::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	device.pImmediateContext->IASetInputLayout(pInputLayout.Get());
}
