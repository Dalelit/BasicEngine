#pragma once

#include "BEDirectXConstantBuffer.h"
#include "BECamera.h"
#include "BEScene.h"
#include <DirectXMath.h>

class BEDXPSConstantBuffer : public BEDirectXConstantBuffer
{
public:
	struct Buffer {
		DirectX::XMVECTOR lightDirection;
		DirectX::XMVECTOR lightColor;
	};

	BEDXPSConstantBuffer(BEDirectXDevice& device);
	void Update(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	D3D11_SUBRESOURCE_DATA constBufferData = {};
	D3D11_BUFFER_DESC bufferDesc = {};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer = nullptr;
};

