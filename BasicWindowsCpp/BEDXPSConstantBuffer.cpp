#include "BEDXPSConstantBuffer.h"

BEDXPSConstantBuffer::BEDXPSConstantBuffer(BEDirectXDevice& device)
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

void BEDXPSConstantBuffer::Update(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera)
{
	HRESULT hr;

	// To do : hack just to get the first light... maybe make a scene light
	buffer.lightColor = pScene->lights[0]->color;
	buffer.lightDirection = pScene->lights[0]->GetDirection();
	//buffer.lightDirection = XMVector3Transform(pScene->lights[0]->GetDirection(), pCamera->GetViewMatrix());

	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXPSConstantBuffer::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}
