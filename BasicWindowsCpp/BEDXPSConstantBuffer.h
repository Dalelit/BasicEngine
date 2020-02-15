#pragma once

#include "pch.h"
#include "BEDirectXConstantBuffer.h"
#include "BECamera.h"
#include "BEScene.h"

class BEDXPSConstantBuffer : public BEDirectXConstantBuffer
{
public:
	struct Buffer {
		DirectX::XMVECTOR ambientColor;
		DirectX::XMVECTOR lightDirection;
		DirectX::XMVECTOR lightDirecitonColor;
		DirectX::XMVECTOR pointLightPosition;
		DirectX::XMVECTOR pointLightColor = {};
		DirectX::XMVECTOR cameraPosition;
	};

	BEDXPSConstantBuffer(BEDirectXDevice& device, BEScene* pScene, BECamera* pCamera);
	void Update(BEDirectXDevice& device);
	void Bind(BEDirectXDevice& device);

private:
	Buffer buffer = {};
	BEScene* pScene;
	BECamera* pCamera;
};

