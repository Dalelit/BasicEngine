#include "BEDXEntityPSConstantBuffer.h"

BEDXEntityPSConstantBuffer::BEDXEntityPSConstantBuffer(BEDirectXDevice& device, BEEntity* pEntity) :
	pEntity(pEntity)
{
	if (pEntity->mesh)
	{
		buffer.textured = pEntity->mesh->IsTextured();
	}
	else buffer.textured = false; // to do: should this be an acceptable outcome?

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

void BEDXEntityPSConstantBuffer::Update(BEDirectXDevice& device)
{
	HRESULT hr;

	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXEntityPSConstantBuffer::Bind(BEDirectXDevice& device)
{
	Update(device);
	device.pImmediateContext->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
}
