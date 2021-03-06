#include "pch.h"
#include "BEDXVSConstantBuffer.h"

BEDXVSConstantBuffer::BEDXVSConstantBuffer(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera) :
	pScene(pScene),
	pCamera(pCamera)
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

void BEDXVSConstantBuffer::Update(BEDirectXDevice& device)
{
	HRESULT hr;

	buffer.transformation = XMMatrixTranspose(pCamera->GetViewProjectionMatrix());

	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXVSConstantBuffer::Bind(BEDirectXDevice& device)
{
	Update(device);
	device.pImmediateContext->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
}
