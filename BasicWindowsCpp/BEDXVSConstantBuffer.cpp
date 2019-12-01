#include "BEDXVSConstantBuffer.h"

BEDXVSConstantBuffer::BEDXVSConstantBuffer(BEDirectXDevice& device)
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

void BEDXVSConstantBuffer::Update(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera)
{
	HRESULT hr;

	buffer.transformation = XMMatrixTranspose(pCamera->GetViewProjectionMatrix());

	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXVSConstantBuffer::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}
