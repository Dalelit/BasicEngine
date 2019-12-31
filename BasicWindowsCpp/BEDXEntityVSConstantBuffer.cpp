#include "BEDXEntityVSConstantBuffer.h"

BEDXEntityVSConstantBuffer::BEDXEntityVSConstantBuffer(BEDirectXDevice& device, BEEntity* pEntity) :
	pEntity(pEntity)
{
	buffer.position = pEntity->position;

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

void BEDXEntityVSConstantBuffer::Update(BEDirectXDevice& device)
{
	HRESULT hr;

	buffer.position = pEntity->position;
	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXEntityVSConstantBuffer::Bind(BEDirectXDevice& device)
{
	Update(device);
	device.pImmediateContext->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
}
