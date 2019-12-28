#pragma once

#include "BEDirectXResource.h"
#include "BECamera.h"
#include "BEScene.h"

class BEDirectXConstantBuffer : public BEDirectXResource
{
public:
	//virtual void Update(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera) = 0;

	// usually called within Bind
	virtual void Update(BEDirectXDevice& device) = 0;

	unsigned int slot = 0;

protected:
	D3D11_SUBRESOURCE_DATA constBufferData = {};
	D3D11_BUFFER_DESC bufferDesc = {};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer = nullptr;
};