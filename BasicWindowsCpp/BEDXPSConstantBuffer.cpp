#include "pch.h"
#include "BEDXPSConstantBuffer.h"

BEDXPSConstantBuffer::BEDXPSConstantBuffer(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera) :
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

void BEDXPSConstantBuffer::Update(BEDirectXDevice& device)
{
	HRESULT hr;

	// To do : hack just to get the first light... maybe make a scene light
	buffer.ambientColor = pScene->ambientLight.color;
	buffer.lightDirecitonColor = pScene->directionalLight.color;
	buffer.lightDirection = pScene->directionalLight.GetDireciton();
	buffer.cameraPosition = pCamera->position;

	if (pScene->lights.size() > 0)
	{
		BEPointLight* pLight = pScene->lights[0];
		buffer.pointLightPosition = pLight->position;
		buffer.pointLightColor = pLight->color;
	}

	hr = device.pDevice->CreateBuffer(&bufferDesc, &constBufferData, &pConstantBuffer);

	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXPSConstantBuffer::Bind(BEDirectXDevice& device)
{
	Update(device);
	device.pImmediateContext->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
}
