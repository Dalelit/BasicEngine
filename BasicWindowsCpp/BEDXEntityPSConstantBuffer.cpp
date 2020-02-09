#include "BEDXEntityPSConstantBuffer.h"

BEDXEntityPSConstantBuffer::BEDXEntityPSConstantBuffer(BEDirectXDevice& device)
{
	constBufferData.SysMemPitch = 0;
	constBufferData.SysMemSlicePitch = 0;
	constBufferData.pSysMem = &buffer;

	bufferDesc.ByteWidth = sizeof(Buffer);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(Buffer);
}

void BEDXEntityPSConstantBuffer::Update(BEDirectXDevice& device, BEEntity& entity)
{
	HRESULT hr;

	BEMaterial* pMaterial = entity.pMaterial;

	assert(pMaterial);

	buffer.ambientColor = pMaterial->ambientColor;
	buffer.diffuseColor = pMaterial->diffuseColor;
	buffer.specularColor = pMaterial->specularColor;
	//buffer.emmissionColor
	buffer.specularExponent = pMaterial->specularExponent;

	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXEntityPSConstantBuffer::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
}
