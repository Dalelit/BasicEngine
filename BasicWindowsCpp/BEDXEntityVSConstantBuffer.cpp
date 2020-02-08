#include "BEDXEntityVSConstantBuffer.h"

BEDXEntityVSConstantBuffer::BEDXEntityVSConstantBuffer(BEDirectXDevice& device)
{
	HRESULT hr;

	constBufferData.SysMemPitch = 0;
	constBufferData.SysMemSlicePitch = 0;
	constBufferData.pSysMem = &buffer;

	bufferDesc.ByteWidth = sizeof(Buffer);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(Buffer);

	hr = device.pDevice->CreateBuffer(&bufferDesc, nullptr, &pConstantBuffer); // creates an empty buffer

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXEntityVSConstantBuffer::Update(BEDirectXDevice& device, BEEntity& entity)
{
	HRESULT hr;

	// to do: merge the camera and model matricies into a single matrix transform
	buffer.transform = XMMatrixTranspose(entity.GetTransform());
	buffer.transformRotation = XMMatrixTranspose(entity.GetTransformRotation());
	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer); // to do: is there an update rather than recreate?

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXEntityVSConstantBuffer::Bind(BEDirectXDevice& device)
{
	//to do: update buffer rather than recreate    device.pImmediateContext->UpdateSubresource(pConstantBuffer.Get(), 0u, nullptr, &constBufferData, 0u, 0u);
	device.pImmediateContext->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
}
