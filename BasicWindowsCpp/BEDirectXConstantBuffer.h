#pragma once

#include "BEDirectXResource.h"
#include "BECamera.h"
#include "BEScene.h"

class BEDirectXConstantBuffer : public BEDirectXResource
{
public:
	unsigned int slot = 0;

protected:
	D3D11_SUBRESOURCE_DATA constBufferData = {};
	D3D11_BUFFER_DESC bufferDesc = {};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer = nullptr;
};

class BEDirectXEntityConstantBuffer : public BEDirectXConstantBuffer
{
public:
	virtual void Update(BEDirectXDevice& device, BEEntity& entity) = 0;
};
